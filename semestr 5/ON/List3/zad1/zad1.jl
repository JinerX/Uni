

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


f(x) = (x-1)

approx, value, it, err = mbisekcji(f, 0.2, 3.14, 0.000000000000000000001, 0.00000000000000000000000001)

println("spproximation: $approx, value: $value, iterations: $it, err: $err")


