using Printf


#=

This will be changed by working UInt64 and reinterpreting bits to avoid using nextflaot

=#


const shift_value = 12


num_examples = 10

function nextfloat64(float_input_val)
    
    #=

    Function is imperfect, it doesn't support some edge cases, when the exponent changes value, but for our purposes (tracking the bit values of Float64 numbers specifically
    when the exponent stays the same) it doesn't matter

    =#

    bits = bitstring(float_input_val)
    # println("input bitstring: $bits")
    sign = bits[1]
    power = bits[2:12]
    signif = bits[13:length(bits)]
    # println("sign: $sign, power: $power, significand: $signif")
    signif = "0"^shift_value * signif
    # println("signif: $signif")
    a = parse(UInt64, signif; base=2)
    a = a + 1
    abits = bitstring(a)
    end_float_bits = sign * power * abits[shift_value+1:length(abits)]
    # println("length of end_float_bits: $(length(end_float_bits))")
    ret_float = parse(UInt64, end_float_bits; base=2)
    ret_float = reinterpret(Float64, ret_float) 
    # ret_bits = bitstring(ret_float)
    # println("end_float: $ret_float, ret_bits: $ret_bits")
    return ret_float
end



function show_first_bitstrings(start, finish, num_examples)
    curr = Float64(start)
    for i in 0:1:num_examples
        if curr > Float64(finish)
            println("not enough examples in the interval, stopping at step $i")
            return
        end
        bits = bitstring(curr)
        print("example: $i; bitstring: $bits; value: $curr")
        @printf("; value(higher precision): %.25e\n", curr)
        curr = nextfloat64(curr)
    end
end

show_first_bitstrings(0.5,1,num_examples)
# a = nextfloat64(Float64(1))
# true_a = nextfloat(Float64(1))
# println("nextfloat64: $a, nextfloat: $true_a, are_equal?: $(a == true_a)")