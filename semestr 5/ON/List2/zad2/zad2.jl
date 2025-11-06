using Plots

f(x) = exp(x)*log(1+exp(-x))

x = 0:0.01:50
y = f.(x)


gr()
p = plot(x,y, label="")
display(p)
# gui()
# readline()