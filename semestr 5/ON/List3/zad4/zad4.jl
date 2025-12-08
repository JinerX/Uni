#Jędrzej Sajnóg, indeks: 279701


include("../approx_module/approx_module.jl")
using .ApproxTools

const AT = ApproxTools

f(x) = sin(x) - (1/2*x)^2
df(x) = cos(x) - x/2

rb, vb, itb, errb = AT.mbisekcji(f, 1.5,2.0,0.5*10^(-5), 0.5*10^(-5))
rn, vn, itn, errn = AT.mstycznych(f, df, 1.5, 0.5*10^(-5), 0.5*10^(-5), 100)
rs, vs, its, errs = AT.msiecznych(f, 1.0,2.0, 0.5*10^(-5), 0.5*10^(-5), 100)


println("Metoda bisekcji: root: $rb, val: $vb, it: $itb, err: $errb")
println("Metoda stycznych: root: $rn, val: $vn, it: $itn, err: $errn")
println("Metoda siecznych: root: $rs, val: $vs, it: $its, err: $errs")
