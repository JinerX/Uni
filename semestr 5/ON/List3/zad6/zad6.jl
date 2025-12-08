#Jędrzej Sajnóg, indeks: 279701


include("../approx_module/approx_module.jl")
using .ApproxTools
const AT = ApproxTools

f1(x) = exp(1-x) - 1
f2(x) = x * exp(-x)

df1(x) = -exp(1-x)
df2(x) = -exp(-x)*(x-1)

δ = 1e-5
ϵ = 1e-5

rb1, vb1, itb1, errb1 = AT.mbisekcji(f1, -0.21,5.0,δ, ϵ)
rb2, vb2, itb2, errb2 = AT.mbisekcji(f2, -5.23,10.128,δ, ϵ)
rn1, vn1, itn1, errn1 = AT.mstycznych(f1,df1, 2.0,δ, ϵ, 400000)
rn2, vn2, itn2, errn2 = AT.mstycznych(f2,df2, 0.8,δ, ϵ, 40)
rs1, vs1, its1, errs1 = AT.msiecznych(f1,-1.0, 0.0,δ, ϵ, 40)
rs2, vs2, its2, errs2 = AT.msiecznych(f2, 1.0,0.5,δ, ϵ, 40)

println("==========f1==========")
println("mbisekcji: root: $rb1, val: $vb1, itera: $itb1, err: $errb1")
println("stycznych: root: $rn1, val: $vn1, itera: $itn1, err: $errn1")
println("siecznych: root: $rs1, val: $vs1, itera: $its1, err: $errs1")

println("==========f2==========")
println("mbisekcji: root: $rb2, val: $vb2, itera: $itb2, err: $errb2")
println("stycznych: root: $rn2, val: $vn2, itera: $itn2, err: $errn2")
println("siecznych: root: $rs2, val: $vs2, itera: $its2, err: $errs2")

