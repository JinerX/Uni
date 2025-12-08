#Jędrzej Sajnóg, indeks: 279701

module ApproxTools
export  mbisekcji, msiecznych, mstycznych


function mbisekcji(f, a::Float64, b::Float64, delta::Float64, epsilon::Float64)
    # returns (r,v,it,err)
    # r - approximation of x for which f(x) = 0
    # v - value f(r)
    # it - number of iterations
    # err - 0 if no error, 1 if function doesn't change the symbol over [a,b] interval
    if (f(a)*f(b) >= 0)
        return (NaN, NaN, 0, 1)
    end
    left = a
    right = b
    it = 0

    while (true)
        it = it + 1
        middle = (right + left) / 2
        if (right - left < delta)
            return (middle, f(middle), it, 0)
        end
        if (abs(f(middle)) < epsilon)
            return (middle, f(middle), it, 0)
        end
        if (f(middle) * f(right) < 0)
            left = middle
        elseif (f(middle) * f(left) < 0)
            right = middle
        end
    end 
end


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


function msiecznych(f, x0::Float64, x1::Float64, delta::Float64, epsilon::Float64, maxit::Int)
    a = x0
    b = x1
    f0 = f(a)
    f1 = f(b)
    if (abs(f0) < epsilon)
        return (x0, f0, 0, 0)
    end
    for k in 1:maxit
        if (abs(f1) > abs(f0))
            f0, f1 = f1, f0
            a, b = b, a
        end
        
        if (abs(f1 - f0) < epsilon)
            return (b,f1,k,2)
        end
        step = (b - a) / (f1 - f0)
        a = b
        f0 = f1
        b = b - f1 * step
        f1 = f(b)

        if (abs(b - a) < delta)
            return (b, f1, k, 0)
        elseif (abs(f1) < epsilon)
            return (b, f1, k, 0)
        end
    end
    return (b, f1, maxit, 1)
end

end