#! /usr/bin/python

import argparse
import os
from os import path
import xml.etree.ElementTree as ET

def inspectFile(filePath):
	tree = ET.parse(filePath)
	root = tree.getroot()
	errors = [err for err in root.iter('error')]
	numErrs = len(errors)
	print 'Scanning file:', filePath+'...', 'OK' if numErrs == 0 else numErrs
	return numErrs



def processLocation(location):
	files = [location+'/'+x for x in os.listdir(location) if path.splitext(x)[1]=='.xml']
	errs = [inspectFile(f) for f in files]
	print ''
	print 'Done scanning files'
	print '--------------------------------------------------------------------'
	totalNumErrors = 0
	for count in errs:
		totalNumErrors += count

	if totalNumErrors == 0:
		print 'Finished successfuly.'
	else:
		print 'Found', totalNumErrors, 'errros. Please inspect files listed above which had errors.'
		exit(1)



if __name__ == '__main__':
	parser = argparse.ArgumentParser()
	parser.add_argument("location", type=str, help='The directory to search for xml files')
	args = parser.parse_args()
	targetPath = path.abspath(args.location)
	print ''
	print 'inspect_reports.py - v0.1'
	print '(c) Guy Sherman, Shermann Innovations 2015'
	print ''
	print 'Inspecting reports in:', targetPath
	print ''
	processLocation(targetPath)
