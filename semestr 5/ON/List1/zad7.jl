# Jędrzej Sajnóg 279701


x0 = Float64(1)

f(x) = sin(x) + cos(3x)
f_deriv(x) = cos(x) - 3*sin(3x)


function deriv_approx(f, x, h)
    xf64 = Float64(x)
    hf64 = Float64(h)
    f_deriv_approx = (f(xf64 + hf64) - f(xf64))/hf64
    return f_deriv_approx
end

ns = 0:1:54
hs = [Float64(2)^-n for n in ns]

true_val = f_deriv(x0)
println("true_val: $true_val")

for h in hs
    println("1+h: $(1+h)")
    approx = deriv_approx(f, x0, h)
    println("approx: $approx, |f'(x0) - ~f'(x0)| = $(abs(true_val - approx))")
end