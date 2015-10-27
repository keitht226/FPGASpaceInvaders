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
		parser.add_argument('fileName', type = str, action = 'store')
		parser.add_argument('varName',type = str, action = 'store')

		#parse arguments
		args = parser.parse_args()

		self.file_name = args.fileName
		self.var_name = args.varName

	def parse_file(self):
		"""Parse .wav file"""
		#open wave file
		w = wave.open(self.file_name, 'rb')
		self.sample_rate = w.getframerate()
		self.num_samples = w.getnframes()

		#write to file
		f = open(self.var_name + 'File.c', 'w')
		f.write('int ' + str(self.var_name) + '_sample_rate = ' + str(self.sample_rate) + ';\n')
		f.write('int ' + str(self.var_name) + '_num_samples = ' + str(self.num_samples) + ';\n')
		f.write('int ' + str(self.var_name) + '_array[] = {\n')
		print self.sample_rate, self.num_samples
		#get frames
		for i in range(0,10):
			frame = w.readframes(1)
			print frame
			#f.write(',' + str(frame))

		f.write('};')


if __name__ == '__main__':
	#initialize object
	converter = wavFileConverter()
	#parse arguments
	converter.parse_arguments()
	#start conversion
	converter.parse_file()
	#converter.convert()