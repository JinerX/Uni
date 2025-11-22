include("../approx_module/approx_module.jl")
using .ApproxTools
const AT = ApproxTools

f(x) = exp(x) - 3*x


r, v, it, err = AT.mbisekcji(f, 0.0,1.0, 1e-4,1e-4)
r2, v2, it2, err2 = AT.mbisekcji(f, 1.0,2.0, 1e-4,1e-4)

println("root1: $r, value: $v, iterations: $it, error: $err")
println("root2: $r2, value: $v2, iterations: $it2, error: $err2")
