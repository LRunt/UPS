import MainWindow

import sys
from PyQt5.QtWidgets import QApplication

if __name__ == '__main__':
    app = QApplication(sys.argv) #app = QApplication([]) - for disabling commnads from command line
    window = MainWindow.MainWindow()
    window.show()
    sys.exit(app.exec_())