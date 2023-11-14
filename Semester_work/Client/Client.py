"""
Author: Lukas Runt
Email: lrunt@students.zcu.cz
Date: 14.11.2023
Version: 0.1.0
Description: Application for TIC-TAC-TOE client
"""
import MainWindow

import sys
from PyQt5.QtWidgets import QApplication

if __name__ == '__main__':
    """
    Entry point of program, showing window of application
    """
    app = QApplication(sys.argv) #app = QApplication([]) - for disabling commnads from command line
    window = MainWindow.MainWindow()
    window.show()
    sys.exit(app.exec_())
