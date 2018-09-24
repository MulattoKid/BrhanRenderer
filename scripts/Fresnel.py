import math
from matplotlib import pyplot as plt

def Fresnel(theta_from, eta_from, eta_to):
	cos_theta_from = math.cos(theta_from)
	sin_theta_from = math.sqrt(1.0 - math.pow(cos_theta_from, 2))
	sin_theta_to = (eta_from * sin_theta_from) / eta_to

	if sin_theta_to >= 1.0:
		return 1.0
	
	cos_theta_to = math.sqrt(1.0 - math.pow(sin_theta_to, 2))

	
	r_parallel = ((eta_to * cos_theta_from) - (eta_from * cos_theta_to)) / ((eta_to * cos_theta_from) + (eta_from * cos_theta_to))
	r_perpendicular = ((eta_from * cos_theta_from) - (eta_to * cos_theta_to)) / ((eta_from * cos_theta_from) + (eta_to * cos_theta_to))

	return (math.pow(r_parallel, 2) + math.pow(r_perpendicular, 2)) / 2.0

x = []
Fr = []
Ft = []
i = 0.0
end = math.pi / 2.0
while i < end:
	x.append(i)
	fr = Fresnel(i, 1.0, 1.5)
	Fr.append(fr)
	Ft.append(1.0 - fr)
	i += 0.01
	
plt.plot(x, Fr)
plt.plot(x, Ft)
plt.show()
