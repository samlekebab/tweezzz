###

import numpy as np
import matplotlib.pyplot as plt
from scipy.fft import fft, fftfreq
import matplotlib.animation as animation

def FFTy(y, duration, sr):
	"""
	sr: sampling rate
	"""
	N = int(duration * sr)
	Y = fft(y)

	return 2.0/N * np.abs(Y[0:N//2])

def FFTx(duration, sr):
	"""
	Y: FFT of waveform
	"""

	N = int(duration * sr)
	T = 1/sr

	return fftfreq(N, T)[:N//2]


def animate(i, line, wavefunc, window_duration, sr, slide_step):
	"""
	i: frame index
	slide_step: duration of one step in the sliding of the window 
	"""

	nb_pts = int(sr * window_duration) # number of points in the window
	nb_slide_step_pts = int(sr * slide_step)

	datay = FFTy(wavefunc[i*nb_slide_step_pts : nb_pts + i*nb_slide_step_pts], window_duration, sr)

	line.set_ydata(datay)

	return line,

def dynamicalFFT(wavefunc, sr, window_duration, slide_step, min_freq, max_freq, frame_duration = 100):
	"""
	"""

	fig, ax = plt.subplots(figsize = (15,20))
	nb_pts = int(sr * window_duration)
	nb_slide_step_pts = int(sr * slide_step)
	ax.set_xlim([min_freq, max_freq])
	ax.set_ylim([0.0, 3000.0])

	freqs = FFTx(window_duration, sr)

	line, = ax.plot(freqs, np.zeros(len(freqs)))

	nb_frames = int((len(wavefunc) - nb_pts) / nb_slide_step_pts)

	ani = animation.FuncAnimation(fig, animate, np.arange(nb_frames), fargs = (line, wavefunc, window_duration, sr, slide_step)
								, interval=frame_duration, blit=True)
	plt.show()

def staticFFT(wavefunc, N, sr, min_freq, max_freq):
	"""
	"""
	figFFT, axFFT = plt.subplots(figsize = (15,20))

	duration = N / sr
	freq = FFTx(duration, sr)
	y = FFTy(wavefunc, duration, sr)

	axFFT.plot(freq, y)
	axFFT.set_xlabel("frequency (Hz)")
	axFFT.set_ylabel("Spectrum")
	axFFT.set_xlim([min_freq, max_freq])

	plt.show()

## TEST SCRIPT ##
test = False
if test:
	SampleRate = 600000000.0
	Freq1 = 10.0e6 #10MHz
	Freq2 = 20.0e6
	DurationOfSignal = 0.2e-4
	N_measure = int(DurationOfSignal * SampleRate)

	tmp_freq = np.ones(N_measure)*Freq1
	wavefunc = np.sin(2*np.pi*np.cumsum(tmp_freq)/SampleRate)

	tmp_freq = np.ones(N_measure)*Freq2
	wavefunc2 = np.sin(2*np.pi*np.cumsum(tmp_freq)/SampleRate + wavefunc[-1])

	wavefunc = np.concatenate((wavefunc, wavefunc2))
	DurationOfSignal = 2*DurationOfSignal

	"""
	figT, axT = plt.subplots(figsize = (15,20))
	axT.plot(np.arange(0, DurationOfSignal, 1/SampleRate), wavefunc)
	axT.set_xlabel("time (in ms)")
	axT.set_ylabel("wavefunc")
	#plt.show()
	"""
	"""
	Y = FFTy(wavefunc, DurationOfSignal, SampleRate)
	X = FFTx(DurationOfSignal, SampleRate)

	figF, axF = plt.subplots(figsize = (15,20))
	axF.plot(X, Y)
	axF.set_xlabel("freq in Hz")
	axF.set_ylabel("spectrum")
	plt.show()
	"""

	dynamicalFFT(wavefunc, SampleRate, 0.1e-4, 0.01e-4, 50e6)
###
f = open("res/cpp.txt")
Y = []
for line in f:
    Y.append(int(line))
###
plt.plot(Y)
plt.show()
###
Yf = FFTy(Y,len(Y),1)
Xf = FFTx(len(Y)/600,600)
plt.plot(Xf,Yf)
plt.show()
###
