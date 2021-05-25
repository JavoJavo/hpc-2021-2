import numpy as np
import matplotlib.pyplot as plt

N = 128
x_values = np.arange(0, N, 1)

x = np.sin((2*np.pi*x_values / 32.0)) # 32 - 256Hz
x += np.sin((2*np.pi*x_values / 64.0)) # 64 - 128Hz

with open('ft.txt') as f:
    lines = f.readlines()

X = []
for line in lines:
    real, imaginary = line.strip().split(' ') 
    real, imaginary = np.double(real), np.double(imaginary)
    X.append(complex(real,imaginary))
    #output.append()  
X = np.array(X)


#X = fft(x)


# Plotting 
_, plots = plt.subplots(2)

## Plot in time domain
plots[0].plot(x)

## Plot in frequent domain
powers_all = np.abs(np.divide(X, N/2))
powers = powers_all[0:N//2]
frequencies = np.divide(np.multiply(8192, np.arange(0, N/2)), N)
plots[1].plot(frequencies, powers)


## Show plots
plt.show()