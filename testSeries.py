import sys
import os
import math
import numpy as np
from scipy import stats
import pylab as plt
from subprocess import call
import webbrowser
import pdfkit


def generateWebAndPdf(fileName,resultTau):
	"Generates an html file with the plots generated and then convert it into a pdf file"
	webName = fileName+'.html'
	f = open(webName,'w')

	title = "<html>\n<title>\n"+"Tau: "+str(sys.argv[1])+" H: "+str(sys.argv[2])+" q: "+str(sys.argv[3])+" N: "+str(N)+"</title>\n"
	header = "<center><h1>Analysis of qNoise</h1></center>\n<font size=\"6\">Parameters:</font> <p>\n"
	values = "<font size=\"5\">"+"Tau: "+str(sys.argv[1])+"  /  H: "+str(sys.argv[2])+"  /  q: "+str(sys.argv[3])+"  /  N: "+str(N)+"</font> <body><p>\n"
	timeSeries1 = "<p style=\"page-break-after:always;\"></p><hr><h2>Time series</h2> <img src=" + fileName+ "_timestep.png"+ " alt=\"Sample\">\n"
	timeSeries2 = "<p style=\"page-break-after:always;\"><hr><h2>Time series zoom</h2> <img src=" + fileName+ "_time.png"+ " alt=\"Sample\">\n"
	histogram = "<p style=\"page-break-after:always;\"><hr><h2>Histogram</h2> <img src=" + fileName+ "_hist.png"+ " alt=\"Histogram\">\n"
	histLog = "<p style=\"page-break-after:always;\"><hr><h2>Histogram semi-log</h2> <img src=" + fileName+ "_histLog.png"+ " alt=\"Semilog histogram\"\n"
	histogram99 = "<p style=\"page-break-after:always;\"><hr><h2>Histogram (no outliers)</h2> <img src=" + fileName+ "_per_hist.png"+ " alt=\"Histogram no outliers\">\n"
	histLog99 = "<p style=\"page-break-after:always;\"><hr><h2>Histogram semi-log (no outliers)</h2> <img src=" + fileName+ "_per_histLog.png"+ " alt=\"Semilog histogram no outliers\">\n"
	footer = "</p>\n</body>\n</html>\n"

	tauString ="<font size=\"5\"> Calculated Tau: %.3f" %resultTau
	tau = float(sys.argv[1])
	difference = math.fabs(tau-resultTau)
	if  difference < (0.1 * tau):
		tauString += " Tau test OK"
	else:
		tauString += " Tau test Failed"
	tauString +="</font><p>\n"
	f.write(header+values+tauString+timeSeries1+timeSeries2+histogram+histLog+histogram99+histLog99+footer)
	f.close()
	print "Generating PDF ",fileName+".pdf"
	pdfkit.from_url(fileName+".html", fileName+'.pdf')



def plotSample(AA,begin,end,extension,step,allsize):
	"Plots a sample of the noise"
	plt.xlabel('Time [s]',fontsize=allsize)
	plt.ylabel('nNoise [arb. units]',fontsize=allsize)
	plt.title('tau: %s, h: %s, q: %s' %(str(tau), str(H),str(q)),fontsize=allsize)
	plotY = AA[int(begin):int(end):step]
	plotX = np.arange(begin*H,end*H,step*H)
	#print begin, end, plotX.shape, plotY.shape
	plt.plot(plotX, plotY, color='k')
	ax = plt.axes()
	for label in (ax.get_xticklabels() + ax.get_yticklabels()):
		label.set_fontsize(allsize)
	plt.tight_layout()
	plt.savefig(fileName+extension+".png")
	plt.close()

def calcTau(AA,tau, H, q, N):
	"Calculates tau from the data. Then compares with the given value."
	lag=1
	A= AA[:-lag]
	B= AA[lag:]

	slope, intercept, r_value, p_value, std_err = stats.linregress(A,B)
	xx= np.arange(A.min(),A.max(),(A.max()-A.min())/len(A))
	print "Calculating Tau from Data."
	tau_calc = (-1 / math.log(slope)) * H
	tau_calc_eff = tau_calc * (5 - 3 * q) /2
	tau_diff = math.fabs(tau_calc - tau)
	#print "Testing Tau from Data."
	#print "slope: ",slope,"\nintercept: ", intercept, "\nr_value: ",r_value, "\np_value: ",p_value, "\nstd_err: ",std_err, "\ntau_exp: ",tau_calc
	print "q= "+str(q)+" tau= "+str(tau)+" tau_calc= "+str(tau_calc)+" tau_calc_eff= "+str(tau_calc_eff)
	if math.fabs(tau_diff < 0.1*tau):
		print "Tau test OK"
	else:
		print "Tau test Failed"
	print ""
	return tau_calc


def calcHist(AA, tau, H, q, N,log=False,percent=False):
	"Calculates the histogram of the data. Then compares it with the theoretical value of the dist."
	allsize = 20
	hist_min= AA.min()
	hist_max=AA.max()
	if percent== True:
		top = np.percentile(AA, 99.95)
		bottom = np.percentile(AA, 0.05)
		AA = AA[ (AA<top) & (AA>bottom)]
		hist_min= AA.min()
		hist_max=AA.max()
	#calculate number of bins using the interquartile range and the Freedman-Diaconis rule
	calcFriedDiac = 0
	if calcFriedDiac == 1:
		iqr = np.subtract(*np.percentile(AA, [75, 25]))
		num_bins=int(2.0 * iqr * N**(1./3.))
	else:
		num_bins=int( N**(1./3.))
	if num_bins > 50:
		num_bins = 50

	#Calculate the histogram and plot
	#print 'Hist Min ',hist_min,' Hist Max ', hist_max,' bins ', num_bins, " N ", N
	#Calculate de histogram
	hist_integer = np.histogram(AA,num_bins,range =(hist_min,hist_max))
	#Normalize the Histogram
	hist_normalized = 1.0 * hist_integer[0] / N
	hist_x = hist_integer[1]
	hist_x = hist_x[:-1]
	#Base vector for the theoretic PDF
	#hist_x = np.arange(hist_min,hist_max,(hist_max-hist_min)/len(hist_normalized))

	#Calculate the theoretic PDF given the parameters
	if (q == 1):
		hist_theoretic = [ math.exp(-  tau * x * x ) for x in hist_x]
	if(q < 1):
		w = math.pow((1 - q) * tau,-.5) #cutoff
		hist_theoretic = [ math.pow((1 -  (x / w)*(x / w) ), (1 / (1 - q)))  for x in hist_x]
	if(q>1):
		hist_theoretic = [ math.pow((1 + tau * (q - 1) * x * x ), (1 / (1 - q))) for x in hist_x]

	#Normalization
	hist_theoretic = [ x / np.sum(hist_theoretic) for x in hist_theoretic]
	#Transform to numpy array
	hist_theoretic = np.asarray(hist_theoretic)
	#Plot
	hist_axes = np.arange(hist_min,hist_max,(hist_max-hist_min)/6)
	hist_axes = [ float(int(10000.* x)/10000.) for x in hist_axes ]
	width = 0.9 * (hist_x[1] - hist_x[0])    # gives histogram aspect to the bar diagram
	ax = plt.axes()
	plt.title('tau: %s, h: %s, q: %s, N: %s' %(str(sys.argv[1]), str(sys.argv[2]),str(sys.argv[3]),str(sys.argv[4])),fontsize=allsize)
	plt.xlabel('Noise [arb. units]',fontsize=allsize)
	plt.ylabel('Histogram',fontsize=allsize)
	if percent== True:
		plt.xlim([hist_min,hist_max])
		plt.ylim()
	#print hist_x.shape, hist_normalized.shape, hist_theoretic.shape
	plt.bar(hist_x, hist_normalized, width,label='Histogram', color='gray',alpha=0.5)
	plt.plot(hist_x, hist_theoretic, color='k',label='Theoretic Dist.', linewidth=2,  linestyle ='--')
	#ax.legend(loc='upper right',fontsize=allsize)
	for label in (ax.get_xticklabels() + ax.get_yticklabels()):
		label.set_fontsize(allsize)
	plt.tight_layout()
	#Calculate the Histogram in Semilog.
	if percent==True:
		name=fileName+"_per"
	else:
		name=fileName
	if (log == 1):
		ax.set_yscale('log')
		plt.savefig(name+"_histLog.png")
	else:
		plt.savefig(name+"_hist.png")
	plt.close()

def callqNoise(tau,H,q,N):
	"Calls the program in c++ which calculates the timeseries"
	os.system("time dist/Debug/GNU-MacOSX/qnoise %s %s %s %s" %(str(tau),str(H),str(q),str(N)))
	fileName= "qNoise_"+str(sys.argv[1])+"_"+str(sys.argv[2])+"_"+str(sys.argv[3])+"_"+str(N)
	return fileName

#Reads the parameters from command line
if len(sys.argv) == 5:
	tau = float( str(sys.argv[1]))
	H = float( str(sys.argv[2]))
	q = float( str(sys.argv[3]))
	N = int( str(sys.argv[4]))
	if N < 1000:
		print "Please use N > 1000"
		sys.exit()
else:
	print 'Use:', str(sys.argv[0]), 'tau, H, q, N'
	sys.exit()

print "Values: tau: ",tau,"H: ", H, "q: ",q, "N: ",N
#calling the qNoise programm (c++)
fileName = callqNoise(tau,H,q,N)
#Reads the file from disk
print "Python: Reading file: ",fileName+".txt"
AA= np.loadtxt(fileName+".txt")

print "var_q ", q, " mean: ", AA.mean(), " stdv: ", AA.std(), " variance: ", AA.std()*AA.std()
#Plots samples of the noise.. one General and a zoom in
print "Plotting noise samples"
plotSample(AA,N/2,N/2+10000,"_time",10,20)
plotSample(AA,0,100000 if N > 100000 else N,"_timestep",10,20)
#Test if the tau is similar to the input
resultTau = calcTau(AA, tau, H, q, N)

#Plots 4 histograms 2 linear, 2 semilog, with and without percentiles.
print "Plotting Histograms"
tau_eff = tau * (5 - 3 * q) /2
calcHist(AA, tau_eff,H, q, N)
calcHist(AA, tau_eff,H, q, N,log=True)
calcHist(AA, tau_eff,H, q, N,percent=True)
calcHist(AA, tau_eff,H, q, N,log=True,percent=True)

#Removes the original noise file (they are very big)
print "Removing file ",fileName+".txt"
DelfileName="rm "+fileName+".txt"
os.system(DelfileName)
#generate report in html and pdf
generateWebAndPdf(fileName,resultTau)


