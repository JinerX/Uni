function msiecznych(f, x0::Float64, x1::Float64, delta::Float64, epsilon::Float64, maxit::Int)
    a = x0
    b = x1
    f0 = f(a)
    f1 = f(b)
    if (abs(f0) < epsilon)
        return (x0, f1, 0, 0)
    end
    for k in 1:maxit
        if (abs(f0) > abs(f1))
            tmp = f0
            f0 = f1
            f1 = tmp
            tmp = a
            a = b
            b = tmp
        end
        
        if (abs(f1 - f0) < epsilon)
            return (a,f1,k,2)
        end
        step = (b - a) / (f1 - f0)
        a = b
        f0 = f1
        b = b - f1 * step
        f1 = f(b)

        if (abs(b - a) < delta)
            return (a, f1, k, 0)
        elseif (abs(f1) < epsilon)
            return (a, f1, k, 0)
        end
    end
    return (a, f1, maxit, 1)
end


f(x) = (x-10)*(x+10)


pred, val, it, err = msiecznych(f, 40.0,60.0, 0.0000000001,0.000000000001, 40)

println("pred: $pred, val: $val, it: $it, error: $err")
