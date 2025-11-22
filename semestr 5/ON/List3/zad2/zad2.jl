

function mstycznych(f, df, x0::Float64, delta::Float64, epsilon::Float64, maxit::Int)
    value = f(x0)
    if (abs(value) < epsilon)
        return (x0, value, 0, 0)
    end
    x_prev = x0
    for k in 1:maxit
        deriv_val = df(x_prev)
        if (abs(deriv_val) < epsilon)
            return (x_prev, value, k, 2)
        end
        x_next = x_prev - value/(deriv_val)
        value = f(x_next)
        if (abs(x_prev - x_next) < delta)
            return (x_next, value, k, 0)
        elseif (abs(value) < epsilon)
            return (x_next, value, k, 0)
        end
        x_prev = x_next
    end
    return (x_prev, value, maxit, 1)
end


f(x) = x^2
df(x) = 2*x

pred, value, it, err = mstycznych(f,df,-6.0, 0.0000000000001,0.00000000000001, 400)
println("x: $pred, value: $value, iterations: $it, error: $err")
