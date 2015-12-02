import numpy as np
import pylab as plt
import sys

#Reads the parameters from command line
if len(sys.argv) == 3:
	fileName = str(sys.argv[1])
	variance = str(sys.argv[2])
else:
	print 'Use:', str(sys.argv[0]), 'fileName'
	sys.exit()

fileName=fileName.replace(".txt","")
#Value of tau depends on q. Correcting so they are independent.
#Reads the file from disk
print "Python: Reading file: ",fileName+".txt"
AA= np.loadtxt(fileName+".txt")
if variance == "v":
	variance = True
	print "Mode Variance"
else:
	variance = False
	print "Mode Tau"

allsize = 20
plt.xlabel('q [arb. units]',fontsize=allsize)
if variance == True:
	plt.ylabel('variance [arb. units]',fontsize=allsize)
else:
	plt.ylabel('tau [arb. units]',fontsize=allsize)
#plt.title('tau: %s, h: %s, q: %s, N: %s' %(str(tau), str(H),str(q),str(int((end-begin)*H))),fontsize=allsize)
plotX = AA[:,0]
plotY = AA[:,1]
xx = np.arange(0,1.66,0.01)
#print begin, end, plotX.shape, plotY.shape
plt.plot(plotX, plotY, marker='s', markeredgecolor='black', markerfacecolor='white',markeredgewidth=2, linestyle='None',markersize=10)
plt.plot(plotX, AA[:,2], 'o' ,color='k',markersize=10)
plt.plot([1.66,1.66], [0,5], '--' ,color='k')
plt.plot(xx, [ 1  for x in xx], '--' ,color='k')
if variance == True:
	plt.plot(xx, [ 1 * (1 /(5 - 3 * x))  for x in xx], '-' ,color='k')
else:
	plt.plot(xx, [ 2 * (1 /(5 - 3 * x))  for x in xx], '-' ,color='k')
ax = plt.axes()
for label in (ax.get_xticklabels() + ax.get_yticklabels()):
	label.set_fontsize(allsize)
if variance == True:
	plt.ylim([0,3])
else:
	plt.ylim([0,3])
	#plt.ylim([0,5])
plt.xlim([0,1.7])
ax.grid(color='k', linestyle=':', linewidth=.5)
plt.tight_layout()
plt.savefig(fileName+".png")
plt.close()
