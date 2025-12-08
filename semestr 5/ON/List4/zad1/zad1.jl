# Jędrzej Sajnóg 279701

function ilorazyRoznicowe(x::Vector{Float64}, f::Vector{Float64})
    differences = copy(f)
    for i in 1:length(x)-1
        for j in length(x):-1:i+1
            differences[j] = (differences[j] - differences[j-1]) / (x[j] - x[j - i])
        end
    end
    return differences
end

 