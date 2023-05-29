###
import time
import numpy as np
import matplotlib.pyplot as plt
###
SampleRate = 6E8
def wait_ramp_up_AOD_1D_v2(self,N_tweezers, Freqs_1D, N_wait_before, N_ramp_up, 
                         N_high, random_seed, optimize, opti_file, plot_power, ax):
    """
    Ramp up the AOD 1D after a waiting time making a chain with the frequencies in Freqs_1D. Keep constant max amplitude
    during N_high.

    Freqs_1D : array of the frequencies (in Hz)
    """
        
    if optimize:
        self.open_opti_file(opti_file)
        
        if (N_tweezers != len(self.RelAmps_1d)) or (N_tweezers != len(self.Phases_1d)):
            raise Exception("Number of tweezers not matching the number of elements in array of optimized quantities.")


    Nsample = N_wait_before + N_ramp_up + N_high
    
# 		print("\nIN 1D AOD - Nsample = " + str(Nsample))

    if Nsample>512*1000000.0:
        #if memory too small for whole sequence
        raise Exception("\nERROR: Sequence is too long")  

    # global amplitude factor
    AmpFactor = np.ones(Nsample)
    AmpFactor[:N_wait_before] = np.zeros(N_wait_before)	
    reduced_N = np.arange(N_ramp_up)/N_ramp_up
    TurnOnVar = 6*reduced_N**5-15*reduced_N**4+10*reduced_N**3
    AmpFactor[N_wait_before:N_wait_before+N_ramp_up] = np.sqrt(TurnOnVar)

    if optimize:
        phases = self.Phases_1d
    else:
        np.random.seed(random_seed)
        phases = np.random.random(N_tweezers)*2*np.pi
        #print("\nMultifreq random relative phases AOD 1D:")
        #print("\tRandom generator Seed = " + str(random_seed))
        #print("\tPhases:")
        #print(phases)

    amp_factor = 32760/N_tweezers
    waveform_1D = np.zeros(Nsample)

    for i in range(N_tweezers):
        phase_form = 2*np.pi*np.cumsum(np.ones(Nsample)*Freqs_1D[i])/SampleRate+phases[i]
# 			print("Phase " + str(i))
# 			print(phase_form)

        if optimize:
            maxAmps_1d = np.max(self.RelAmps_1d)
            waveform_1D += amp_factor * self.RelAmps_1d[i]/maxAmps_1d * np.sin(phase_form)
        else:
            waveform_1D += amp_factor*np.sin(phase_form)

        del phase_form
        #gc.collect()

    if plot_power:
        stock_to_plot_pow1D = Bin_single_array(AmpFactor, self.nb_points_binned)
        t_to_plot = Bin_single_array(np.arange(Nsample)*(1/self.SampleRate), self.nb_points_binned)
        ax.plot(t_to_plot*1e3, (stock_to_plot_pow1D/np.max(stock_to_plot_pow1D))**2, label="Power AOD 1D", color = 'tab:red')


    waveform_1D = AmpFactor*waveform_1D
    #waveform_1D, rescaling_factor1D = self.check_array_boundaries_v3(waveform_1D, "waveform AOD 1D.")

    return np.int16(waveform_1D)#, rescaling_factor1D, Nsample

###
MaxAr = 512*1000000

def delta(ar1,ar2):
    if ar1.size!=ar2.size:
        raise Exception("ar1 and ar2 ar'nt the same size, %d %d"% (ar1.size,ar2.size))
    plotMaxSample = 2000 
    size = ar1.size
    res = np.zeros(size)
    for k in range(size):
        i=int(k / size * plotMaxSample)
        res[i]=min(10,max(res[i],abs(ar1[k]-ar2[k])))

    return res
###
#def wait_ramp_up_AOD_1D_v2(self, N_tweezers, Freqs_1D, N_wait_before, N_ramp_up, 
#                         N_high, random_seed, optimize, opti_file, plot_power, ax):

output = wait_ramp_up_AOD_1D_v2(self = "null",N_tweezers = 1,Freqs_1D = [1E6],N_wait_before = 1000,N_ramp_up = 100000,N_high = 1000000, random_seed = False, optimize = False, opti_file = "null", plot_power=False,ax = "null")
output2 = wait_ramp_up_AOD_1D_v2(self = "null",N_tweezers = 2,Freqs_1D = [1E6,1E6+1E2],N_wait_before = 1000,N_ramp_up = 100000,N_high = 1000000, random_seed = False, optimize = False, opti_file = "null", plot_power=False,ax = "null")
###sampling capabilities at 600MG/s capable by python in this context : 0.107, not realtime perf
stampedTime = time.process_time()
i=0
deltaTime=30
while time.process_time()<stampedTime+deltaTime:
    wait_ramp_up_AOD_1D_v2(self = "null",N_tweezers = 1,Freqs_1D = [1E6],N_wait_before = 1_000,N_ramp_up = 1_000_000,N_high = 100_000_000, random_seed = False, optimize = False, opti_file = "null", plot_power=False,ax = "null")
    i+=1
print(i*101_001_000 / SampleRate / deltaTime )
#plt.plot(output,output2)
#plt.show()
###
#deltaAr = delta(output,output2)
#plt.plot(deltaAr,output,output2)
#plt.show()
###

