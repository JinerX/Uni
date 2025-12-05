# function warNewton(x::Vector{Float64}, fx::Vector{Float64}, t::Float64)
#     product = fx[1]
#     for i in 2:length(x)
#         product = t * product + fx[i]
#     end
#     return product
# end

function warNewton(x::Vector{Float64}, fx::Vector{Float64}, t::Float64)
    product = fx[length(fx)]
    for i in (length(fx)-1):-1:1
        product = product * (t - x[i]) + fx[i]
    end
    return product
end