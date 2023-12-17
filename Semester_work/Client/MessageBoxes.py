from PyQt5.QtWidgets import QMessageBox


def show_error_message(message):
    """
    Method to show error message to the user
    :param message: Message that will be shown to the user
    """
    error_message = QMessageBox()
    error_message.setIcon(QMessageBox.Critical)
    error_message.setText(message)
    error_message.setWindowTitle("Error")
    error_message.setStandardButtons(QMessageBox.Ok)
    error_message.exec_()
