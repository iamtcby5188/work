#!/usr/bin/python
# -*- coding: utf-8 -*-

import ftplib
import platform
import socket
import os
import sys
import traceback
import logging
from stat import *

HOST = '192.168.1.54'
USER = 'ubuntu'
PASSWD = 'ubuntu'

class FTPHandler:
    
    def __init__(self, host, user="", passwd="", port = 21):
        '''
        Init the parameters: host, user, passwd, filepath, filename
        '''
        self.host = host
        self.user = user
        self.passwd = passwd
        if user == '':
            self.user = 'anoymous'
            self.passwd = ''
        self.port = port
        self.ftp = None    

    def open_ftp(self):
        try:
            self.ftp = ftplib.FTP(host=self.host, 
                                  user=self.user, 
                                  passwd=self.passwd)
            self.ftp.connect(self.host, self.port)
        except(socket.error, socket.gaierror) as e:
            logging.error('ERROR: cannont  reach (%s)' % self.host)
            logging.error(traceback.format_exc())
            traceback.print_exc()
            return False
        logging.info('Connected to host (%s)' % self.host)
        return True
    
    def login(self):
        try:
            self.ftp.login(user=self.user, 
                           passwd=self.passwd)
        except ftplib.error_perm as e:
            logging.error(traceback.format_exc())
            traceback.print_exc()
            self.quit()
            return False
        logging.info('*** Logged in as (%s)' % self.user)
        return True

    def quit(self):
        if self.ftp is not None:
            self.ftp.quit

    def change_working_dir(self, pathname):
        try:
            self.ftp.cwd(pathname)
        except:
            logging.error(traceback.format_exc())
            traceback.print_exc()
            return False
        return True

    def make_dir(self, pathname):
        try:
            self.ftp.mkd(pathname)
        except:
            logging.error(traceback.format_exc())
            traceback.print_exc()
            return False
        return True

    def get_size(self, filename):
        file_size = -1
        try:
            file_size = self.ftp.size(filename)
        except:
            logging.error(traceback.format_exc())
            traceback.print_exc()
        return file_size

    def download(self, filename):
        try:
            self.download_filesize = self.ftp.size(filename)
            self.byte_recieved = 0.0
            self.download_file_handler = open(filename, 'wb')
            self.ftp.retrbinary('RETR %s' % filename,
                                self.download_callback)
            self.download_file_handler.close()
        except:
            logging.error(traceback.format_exc())
            traceback.print_exc()
            os.unlink(filename)
            return 
        else:
            logging.debug('*** Downloading Completed (%s)' % filename)
    
    def download_callback(self, data):
        self.byte_recieved += len(data)
        logging.debug('Downloading... %.2f%%' % (self.byte_recieved / self.download_filesize * 100))
        self.download_file_handler.write(data)        

    def upload(self, filepath, filename):
        try:
            fullpathname = '%s/%s' % (filepath, filename) # assume linux

            if platform.system().find('Windows') != -1: # Windows
                fullpathname = '%s\%s' % (filepath, filename)

            f = open(fullpathname, 'r')
            self.upload_filesize = os.stat(fullpathname)[ST_SIZE]
            self.byte_sent = 0.0
            self.ftp.storbinary(cmd="STOR %s" % filename,
                                fp=f,
                                callback=self.upload_callback)
        except:
            print '*** Uploading Failed (%s) - %s:%s (%s/%s)' % (fullpathname, self.host, self.port, self.user, self.passwd)
            logging.debug(traceback.format_exc())
            #traceback.print_exc()
        else:
            print '*** Uploading Completed (%s) - %s:%s (%s/%s)' % (fullpathname, self.host, self.port, self.user, self.passwd)
            logging.info('*** Uploading Completed (%s) - %s:%s (%s/%s)' % (fullpathname, self.host, self.port, self.user, self.passwd))
    
    def upload_callback(self, data):
        self.byte_sent += len(data)
        logging.debug('Uploading... %.2f%%' % (self.byte_sent / self.upload_filesize * 100))

if __name__ == '__main__':
    if len(sys.argv) != 7:
        print 'Insufficient arguments, usage example: python ftphandler.py <ip> <port> <username> <password> <file_path> <file_name>'
        exit()
    if not os.path.exists('./log'):
        os.makedirs('./log')
    logging.basicConfig(filename='./log/ftphandler.log', level=logging.DEBUG)
    host = sys.argv[1].lstrip('[').rstrip(']')
    port = sys.argv[2].lstrip('[').rstrip(']')
    username = sys.argv[3].lstrip('[').rstrip(']')
    password = sys.argv[4].lstrip('[').rstrip(']')
    file_path = sys.argv[5].lstrip('[').rstrip(']')
    file_name = sys.argv[6].lstrip('[').rstrip(']')
    try:
        ftp_handler = FTPHandler(host, username, password, port)
        if ftp_handler.open_ftp() and ftp_handler.login():
            #ftp_handler.change_working_dir('ebooks')
            #ftp_handler.download('5.txt')
            #ftp_handler.upload('.', '5.txt')
            ftp_handler.upload(file_path, file_name)
            ftp_handler.ftp.retrlines('LIST')
            ftp_handler.quit()
    except Exception, e:
        print '*** Uploading Failed (%s) - %s:%s (%s/%s)' % (file_name, host, port, username, password)
        #traceback.print_exc()
        logging.debug(traceback.format_exc())
