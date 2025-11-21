include("../approx_module/approx_module.jl")
using .ApproxTools
const AT = ApproxTools

f(x) = exp(x) - 3*x


r, v, it, err = AT.mbisekcji(f, 0.0,1.0, 1e-4,1e-4)
println("root: $r, value: $v, iterations: $it, error: $err")
