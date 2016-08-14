#include "motion_controller.h"
#include "msg_types.h"
#include <cstring>

extern MotionController motionController;
volatile bool velo_update_flag;

void timer_callback()
{
    static uint32_t cnt = 0;
    motionController.stepper_update();

    if (cnt%constants::VeloUpdateCnt==0)
    {
        motionController.velo_update_flag_ = true;
    }
    cnt++;
}

MotionController::MotionController()
{
    operating_mode_ = constants::OpModeCmd;
    velo_update_flag_ = false;
    velo_quit_flag_ = false;
}


void MotionController::initialize()
{
    for (int i =0; i<constants::NumMotor; i++)
    {
        uint8_t clkPin = constants::MotorPinArray[i].clk;
        uint8_t dirPin = constants::MotorPinArray[i].dir;

        motor_[i] = Stepper(clkPin, dirPin); 
        motor_[i].initialize();
        motor_[i].set_velocity(0.0);
    }

    pinMode(constants::DriveEnablePin,OUTPUT);
    digitalWrite(constants::DriveEnablePin, HIGH);
}


void MotionController::update()
{
    switch (operating_mode_)
    {
        case constants::OpModeCmd:
            cmd_mode_update();
            break;

        case constants::OpModeVel:
            vel_mode_update();
            break;

        default:
            // Shouldn't be here - set mode to command
            operating_mode_ = constants::OpModeCmd;
            break;

    }
}


// Protected methods
// ----------------------------------------------------------------------------
void MotionController::cmd_mode_update()
{ 
    CmdMsg cmd_msg;
    RspMsg rsp_msg;
    int num_bytes = RawHID.recv(&cmd_msg,constants::CmdModeMsgTimeout);

    if (num_bytes != sizeof(cmd_msg))
    {
        rsp_msg.msg_id = constants::ErrMsgId;
        rsp_msg.cmd_id = constants::CmdError;
        rsp_msg.success = false;
        char msg[] = "too few bytes in message";
        snprintf(rsp_msg.message,sizeof(msg),msg);
        RawHID.send(&rsp_msg,constants::CmdModeMsgTimeout);
        return;
    }

    if (cmd_msg.msg_id != constants::CmdMsgId)
    {
        rsp_msg.msg_id = constants::ErrMsgId;
        rsp_msg.cmd_id = constants::CmdError;
        rsp_msg.success = false;
        char msg[] = "excpected CmdMsgId";
        snprintf(rsp_msg.message,sizeof(msg),msg);
        RawHID.send(&rsp_msg,constants::CmdModeMsgTimeout);
        return;
    }

    bool success = true;
    switch (cmd_msg.cmd_id)
    {
        case constants::CmdStartVelCtrl:
            operating_mode_ = constants::OpModeVel;
            time_us_ = 0;
            micros_last_ = micros();
            velo_update_flag_ = false;
            velo_quit_flag_ = false;
            velo_timer_.priority(1);
            velo_timer_.begin(timer_callback, constants::TimerPeriod);
            break;

        case constants::CmdSetMotorPos:
            int32_t pos[constants::NumMotor];
            std::memcpy(pos,cmd_msg.data,sizeof(pos));
            for (int i=0; i<constants::NumMotor; i++)
            {
                motor_[i].set_velocity(0.0);
                motor_[i].set_position(pos[i]);
            }
            break;

        case constants::CmdSetMotorVel:
            float vel[constants::NumMotor];
            std::memcpy(vel,cmd_msg.data,sizeof(vel));
            for (int i=0; i<constants::NumMotor; i++)
            {
                motor_[i].set_velocity(vel[i]);
            }
            break;

        default:
            success = false;
            break;
    }

    rsp_msg.msg_id = constants::CmdMsgId;
    rsp_msg.cmd_id = cmd_msg.cmd_id;
    rsp_msg.success = success;
    if (!success)
    {
        char msg[] = "uknown cmd_id";
        snprintf(rsp_msg.message,sizeof(msg),msg);
    }
    RawHID.send(&rsp_msg,constants::CmdModeMsgTimeout);
}


void MotionController::vel_mode_update()
{
    int num_bytes;
    static int cnt = 0;

    if (velo_update_flag_)
    {
        velo_update_flag_ = false;

        if (velo_quit_flag_)
        {
            velo_timer_.end();
            operating_mode_ = constants::OpModeCmd;
        }

        // Compute time using difference
        uint32_t micros_curr = micros();
        uint32_t micros_dt = micros_curr - micros_last_;
        micros_last_ = micros_curr;
        time_us_ += uint64_t(micros_dt);

        // Send time and position data
        PosMsg pos_msg;
        pos_msg.msg_id = constants::PosMsgId;
        pos_msg.time = time_us_;
        for (int i=0; i<constants::NumMotor; i++)
        {
            pos_msg.position[i] = motor_[i].position();
        }
        pos_msg.quit_flag = velo_quit_flag_;
        num_bytes = RawHID.send(&pos_msg,constants::VelModeMsgTimeout);
        if (num_bytes != sizeof(pos_msg))
        {
            return; // Is this the right thing to do ...
        }

        // Receive Velocity update
        VelMsg vel_msg;
        num_bytes = RawHID.recv(&vel_msg,constants::VelModeMsgTimeout);
        if (num_bytes != sizeof(vel_msg))
        {
            return; // Is this the right thing to do ...
        }

        //Serial.print(micros_dt);
        //Serial.println();

        for (int i=0; i<constants::NumMotor; i++)
        {
            motor_[i].set_velocity(vel_msg.velocity[i]);
        }
        velo_quit_flag_ = vel_msg.quit_flag;
    }
}

