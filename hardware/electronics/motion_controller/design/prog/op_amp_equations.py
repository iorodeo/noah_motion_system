from __future__ import print_function 
from sympy import *

disp_line_len = 140

V1, V2, Vop, Vout, Vref, Vcom, Vdif = symbols('V1 V2 Vop Vout Vref Vcom Vdif') 
Rin, Rf, Rg = symbols('Rin, Rf, Rg')

eqn1 = Eq((V1 - Vop)/Rin, (Vop - Vout)/Rf)
eqn2 = Eq((V2 - Vop)/Rin + (Vref - Vop)/Rin, Vop/Rg)

print()
print('-'*disp_line_len)
print('* Equation #1')
print()
pretty_print(eqn1)

print()
print('-'*disp_line_len)
print('* Equation #2')
print()
pretty_print(eqn2)

#Vop_from_eqn1 = solve(eqn1, Vop)[0]
#Vop_from_eqn2 = solve(eqn2, Vop)[0]

Vop_from_eqn1 = list(solveset(eqn1, Vop))[0]
Vop_from_eqn2 = list(solveset(eqn2, Vop))[0]

print()
print('-'*disp_line_len)
print('* Solve equation #1 for Vop')
print()
pretty_print(Eq(Vop, Vop_from_eqn1))
print()

print()
print('-'*disp_line_len)
print('* Solve equation #2 for Vop')
print()
pretty_print(Eq(Vop, Vop_from_eqn2))
print()

eqn3 = Eq(Vop_from_eqn1, Vop_from_eqn2)

print()
print('-'*disp_line_len)
print('* Set expression for Vop from equations 1 and 2 equal to each other to get equation 3')
print()
pretty_print(eqn3)
print()

#Vout_from_eqn3 = solve(eqn3, Vout)[0]
Vout_from_eqn3 = list(solveset(eqn3, Vout))[0]
Vout_from_eqn3 = expand(Vout_from_eqn3)
Vout_from_eqn3 = simplify(Vout_from_eqn3)

print()
print('-'*disp_line_len)
print('* Solve equation 3 for Vout')
print()
pretty_print(Eq(Vout, Vout_from_eqn3))
print()

eqn_Vdif = Eq(V1, Vcom - Vdif/2)
eqn_Vcom = Eq(V2, Vcom + Vdif/2)

print()
print('-'*disp_line_len)
print('* Define Vcom and Vdif as follows')
print()
pretty_print(eqn_Vdif)
print()
pretty_print(eqn_Vcom)
print()

Vout_from_eqn3 = Vout_from_eqn3.subs(V1, Vcom - Vdif/2)
Vout_from_eqn3 = Vout_from_eqn3.subs(V2, Vcom + Vdif/2)

print('-'*disp_line_len)
print('* Sub in expressions for V1 and V2  in terms of Vdif and Vcom')
print()
pretty_print(Eq(Vout, Vout_from_eqn3))
print()

print('-'*disp_line_len)
print('* Enumerate args of Vout expression LHS')
print()
for i, val in enumerate(Vout_from_eqn3.args):
    print('args[{0}] = '.format(i))
    print()
    pretty_print(val)
    print()

numer = Vout_from_eqn3.args[2]

print('-'*disp_line_len)
print('* Examine numerator, which is arg[2]')
print()
pretty_print(numer)
print()

numer = numer.expand().collect([Vdif, Vcom, Vref])

print('-'*disp_line_len)
print('* Exand and collect w.r.t Vdif, Vcom, Vref')
print()
pretty_print(numer)
print()

Vcom_coeff = numer.coeff(Vcom)

print('-'*disp_line_len)
print('* Get coefficient of Vcom')
print()
pretty_print(Vcom_coeff)
print()

Vcom_coeff_eqn = Eq(Vcom_coeff,0)

print('-'*disp_line_len)
print('* Set Equal to zero and solve for Rg')
print()
pretty_print(Vcom_coeff_eqn)
print()

#Rg_sol = solve(Vcom_coeff_eqn,Rg)[0]
Rg_sol = list(solveset(Vcom_coeff_eqn,Rg))[0]

print('-'*disp_line_len)
print(' Bias network resistor must satisfy this equation to get rid of Vcom')
print()
pretty_print(Eq(Rg, Rg_sol))
print()

Vout_from_eqn3 = Vout_from_eqn3.subs(Rg,Rg_sol)
Vout_from_eqn3 = Vout_from_eqn3.simplify()

print('-'*disp_line_len)
print('* Sub in value for Rg into expression for Vout and simplify to get expression for Vout')
print()
pretty_print(Eq(Vout, Vout_from_eqn3))
print()




