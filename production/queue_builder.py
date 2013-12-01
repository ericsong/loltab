#!/usr/bin/python
import os
import thread

exitFlag = 0

class QueueNode():
	def __init__(self, data = None, Next = None):
		self.data = data
		self.Next = Next

class Queue():
	def __init__(self):
		self.front = None
		self.back = None
		self.size = 0
		self.lock = threading.Lock()
	def enqueue(self, data):
		if (self.front == None):
			self.front = QueueNode(data)
			self.back = self.front
		else:
			self.back.Next = QueueNode(data)
			self.back = self.back.Next
		self.size += 1
	def dequeue(self):
		if (self.front == None):
			return None
		else:
			data = self.front.data
			self.front = self.front.Next
			self.size -= 1
			return data
	def peek(self):
		if (self.front == None):
			return None
		else:
			return self.front.data

def addToQueue(queue):
	index = 0
	while not exitFlag:
		directory = "img" + str(index) + ".png"
		while not directory in os.listdir("."):
			pass
		queue.lock.acquire()
		queue.enqueue(directory)
		if (queue.size > 100):
			fname = queue.dequeue()
			if directory in os.listdir("."):
				os.system("rm " + directory)
		queue.lock.release()
		index += 1

def detectScoreboard(images, extractions):
	while not exitFlag:
		images.lock.acquire()
		item = images.dequeue()
		images.lock.release()
		if (item != None):
			p = Popen("./detect " + item, stdout = PIPE, stdin = PIPE, stderr = STDOUT)
			output = p.stdout.read()
			if (output != "False"):
				extractions.lock.acquire()
				extractions.enqueue(item)
				extractions.lock.release()

def extractScoreboard(extractions, data_q):
	while not exitFlag:
		extractions.lock.acquire()
		item = extractions.dequeue()
		extractions.lock.release()
		if (item != None):
			p = Popen("./extract " + item, stdout = PIPE, stdin = PIPE, stderr = STDOUT)
			output = p.stdout.read()
			if (output != "False"):
				data_q.lock.acquire()
				data_q.enqueue(output)
				data_q.lock.release()

def extractData(data_q):
	while not exitFlag:
		data_q.lock.acquire()
		item = data_q.dequeue()
		data_q.lock.release()
		if (item != None):
			p = Popen("./DisplayImage " + item, stdout = PIPE, stdin = PIPE, stderr = STDOUT)
			os.system("rm " + item)

def main():
	image_queue = Queue()
	extract_queue = Queue()
	data_queue = Queue()
	try:
		thread.start_new_thread(addToQueue, (image_queue, ))
		thread.start_new_thread(detectScoreboard, (image_queue, extract_queue, ))
		thread.start_new_thread(extractScoreboard, (extract_queue, data_queue, ))
		thread.start_new_thread(extractData, (data_queue, ))
	except:
		print "Error creating threads"
	while (True):
		print "Image queue size: " + image_queue.size

if __name__ == "__main__":
	main()
