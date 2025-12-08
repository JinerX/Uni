#Jędrzej Sajnóg, indeks: 279701


include("../approx_module/approx_module.jl")
using .ApproxTools
const AT = ApproxTools

δ = 1e-5
ϵ = 1e-5

f1(x) = exp(1-x) - 1
f2(x) = x * exp(-x)

df1(x) = -exp(1-x)
df2(x) = -exp(-x)*(x-1)
r1, v1, it1, err1 = AT.mstycznych(f1,df1, 2.0, δ, ϵ, 1000)
r5, v5, it5, err5 = AT.mstycznych(f1,df1, 5.0, δ, ϵ, 1000)
r7, v7, it7, err7 = AT.mstycznych(f1,df1, 7.0, δ, ϵ, 1000)
r10, v10, it10, err10 = AT.mstycznych(f1,df1, 10.0, δ, ϵ, 1000)
rn2, vn2, itn2, errn2 = AT.mstycznych(f2,df2, 1.1,δ, ϵ, 100)
rn23, vn23, itn23, errn23 = AT.mstycznych(f2,df2, 3.0,δ, ϵ, 100)
rn210, vn210, itn210, errn210 = AT.mstycznych(f2,df2, 10.0,δ, ϵ, 100)
rn220, vn220, itn220, errn220 = AT.mstycznych(f2,df2, 20.0,δ, ϵ, 100)



println("========f1=============")
println("stycznych: x0:$(2.0)  root: $r1, val: $v1, itera: $it1, err: $err1")
println("stycznych: x0:$(5.0)  root: $r5, val: $v5, itera: $it5, err: $err5")
println("stycznych: x0:$(7.0)  root: $r7, val: $v7, itera: $it7, err: $err7")
println("stycznych: x0:$(10.0)  root: $r10, val: $v10, itera: $it10, err: $err10")

println("============f2============")
println("stycznych: x0:$(1.1)  root: $rn2, val: $vn2, itera: $itn2, err: $errn2")
println("stycznych: x0:$(3.0)  root: $rn23, val: $vn23, itera: $itn23, err: $errn23")
println("stycznych: x0:$(10.0)  root: $rn210, val: $vn210, itera: $itn210, err: $errn210")
println("stycznych: x0:$(20.0)  root: $rn220, val: $vn220, itera: $itn220, err: $errn220")
