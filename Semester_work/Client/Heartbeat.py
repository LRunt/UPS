"""
Author: Lukas Runt
Email: lrunt@students.zcu.cz
Date: 18.12.2023
Version: 0.1.0
Description: Heartbeat sends ping messages in interval when user is connected
"""

import threading
import time
from Logger import logger
from Constants import MESSAGE_PING, END_OF_MESSAGE


class Heartbeat(threading.Thread):
    def __init__(self, socket_instance, time_interval):
        super().__init__()
        self.socket_instance = socket_instance
        self.time_interval = time_interval
        self.stop_event = threading.Event()

    def run(self):
        """
        Method sends ping message in time_interval
        """
        logger.info("Heartbeat started")
        while not self.stop_event.is_set():
            try:
                # send heartbeat message and wait
                logger.info(f"Sending PING message")
                self.socket_instance.send(f"{MESSAGE_PING}{END_OF_MESSAGE}".encode())
                time.sleep(self.time_interval)
            except Exception as e:
                logger.error(f"Heartbeat failed: {str(e)}")
                break

    def stop(self):
        """
        Stop the Heartbeat thread
        """
        self.stop_event.set()
