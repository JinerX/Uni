function newton_to_natural(c, x)
    n = length(c)
    a = zeros(Float64, n)
    prod = zeros(Float64, n)

    a[1] = c[1]
    prod[1] = 1.0

    for k in 2:n
        for i in (k-1):-1:1
            prod[i+1] = prod[i] - x[k-1] * prod[i+1]
        end
        prod[1] = -x[k-1] * prod[1]

        for i in 1:k
            a[i] += c[k] * prod[i]
        end
    end

    return a
end

