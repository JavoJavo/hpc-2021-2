from math import e,pi

def rotating_unit_circle(t,f=1):
    return e**(2*pi*complex(0,1)*t*f)

def WN(N):
    return e**(-complex(0,1)*((2*pi)/N))

# SYMMETRY:
# ex.: WN(8)**3 = WN(8)**7
# where N = 8 and k's start from 0 and end in 7