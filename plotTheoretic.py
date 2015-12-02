#Calculate the theoretic PDF given the parameters
import numpy as np
import pylab as plt
import math


#Add some constants, I have played around with them to make the figure pretty.
textSize=20
tau = 10.0
H = 0.1
hist_min = -1.5
hist_max = 1.5
num_points=1001.0
#Switch to change between semilog and normal graphics
log = True
name="theoretic"
#for in q between 0 and 1.6
for q in np.arange(0,1.7,0.1):
#Generate a vector to store the abscissas.
	step=(hist_max-hist_min)/num_points
	hist_x = np.arange(hist_min,hist_max,step)
#for q=1 the formula is the gaussian
	if (q == 1.0):
		hist_theoretic = [ math.exp(- tau * x * x) for x in hist_x]
	if(q < 1):
#for q<1 the functions don't take the whole dominion
#they will have the same number of points anyways with more density.
		w = math.pow((1 - q) * tau,-.5) #cutoff
		step=(2.0*w)/num_points
		hist_x = np.arange(-w,w,step)
		hist_theoretic = [ math.pow((1 -  (x / w)*(x / w) ), (1 / (1 - q)))  for x in hist_x]
	if(q>1):
		hist_theoretic = [ math.pow((1 + tau * (q - 1) * x * x ), (1 / (1 - q))) for x in hist_x]
	print "q: ", q, " H: ", H, " tau: ", tau , " hist_x: ", hist_x.shape
#Normalization is just the sum, is the same for all types of histogram
	hist_theoretic = [ x / np.sum(hist_theoretic) for x in hist_theoretic]
#Transform to numpy array
	hist_theoretic = np.asarray(hist_theoretic)

	#Plot
	hist_axes = np.arange(hist_min,hist_max,(hist_max-hist_min)/6)
	hist_axes = [ float(int(10000.* x)/10000.) for x in hist_axes ]
	width = 0.9 * (hist_x[1] - hist_x[0])    # gives histogram aspect to the bar diagram
	ax = plt.axes()
	plt.xlabel('x [arb. units]',fontsize=textSize)
	plt.ylabel('PDF(x)',fontsize=textSize)

#plots the data.
#q<1
	if(q < 1):
		plt.plot(hist_x, hist_theoretic,color='k', linewidth=1,  linestyle =':')
#q=1, I'm giving this a tolerance of 0.001 as I'm comparing float.
#notice the label.
	if (np.fabs(q -1.0)<0.001):
		plt.plot(hist_x, hist_theoretic,color='k', linewidth=2,  linestyle ='-', label='q = 1')
#q>1
	if(q>1):
		plt.plot(hist_x, hist_theoretic,color='k', linewidth=.5,  linestyle ='-')

#I'm using a label per case so I have to replot two examples of the other data just for the label.
	if(np.fabs(q -1.2)<0.001):
		plt.plot(hist_x, hist_theoretic,color='k', linewidth=.5,  linestyle ='-', label='q > 1')
	if(np.fabs(q -0.5)<0.001):
		plt.plot(hist_x, hist_theoretic,color='k', linewidth=1,  linestyle =':', label='q < 1')


for label in (ax.get_xticklabels() + ax.get_yticklabels()):
	label.set_fontsize(textSize)

#print labels in the upper right
	ax.legend(loc='upper right',fontsize=textSize)
#add a grid
	ax.grid(color='k', linestyle='--', linewidth=.5)
#limit the y axis.
	plt.ylim([10E-6,0.006])

	plt.tight_layout()

#Calculate the Histogram in Semilog. Save and exit
if (log == True):
	ax.set_yscale('log')
	plt.savefig(name+"_histLog.png")
else:
	plt.savefig(name+"_hist.png")
plt.close()
