import argparse
import os
import wave


"""wavFileConverter filename.wav whatToCallTheSound"""


class wavFileConverter:

	def __init__ (self):
		self.args = None
		self.file_name = ""
		self.var_name = ""
		self.num_samples = 0
		self.sample_rate = 0


	def parse_arguments(self):
		"""Parse Command line arguments"""
		#setup parser
		parser = argparse.ArgumentParser()
		parser.add_argument('fileName', type = int, action = 'store')
		parser.add_argument('varName',type = int, action = 'store')

		#parse arguments
		args = parser.parse_args()

		self.file_name = args.fileName
		self.var_name = args.varName

	def parse_file(self):
		"""Parse .wav file"""
		w = wave.open(self.file_name, 'r')
		self.sample_rate = w.getframerate()
		self.num_samples = w.getnframes()
		#for i in range(0,self.num_samples)
			#frame = w.readframes(i)
			#print frame


if __name__ == '__main__':
	#initialize object
	converter = wavFileConverter()
	#parse arguments
	converter.parse_arguments()
	#start conversion
	converter.convert()