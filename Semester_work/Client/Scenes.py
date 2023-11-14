from PyQt5.QtWidgets import QWidget,  QLabel, QLineEdit, QVBoxLayout, QPushButton

class LoginScene(QWidget):
    def __init__(self):
        super().__init__()
        self.text_field_IP_address = QLineEdit(self)
        self.text_field_port = QLineEdit(self)
        self.text_field_username = QLineEdit(self)
        self.login_button = QPushButton('Login', self)
        #self.login_widget =
        self.initUI()

    def initUI(self):
        # Create labels and text fields for the first scene
        label_IP_address = QLabel('IP address:')
        label_port = QLabel('Port:')
        label_username = QLabel('Username:')

        # Create button to switch to the next scene
        #switch_button.clicked.connect(self.login_widget)

        # Create layout for the first scene
        login_layout = QVBoxLayout()
        login_layout.addWidget(label_IP_address)
        login_layout.addWidget(self.text_field_IP_address)
        login_layout.addWidget(label_port)
        login_layout.addWidget(self.text_field_port)
        login_layout.addWidget(label_username)
        login_layout.addWidget(self.text_field_username)
        login_layout.addWidget(self.login_button)

        # Create widget for the first scene
        self.login_widget = QWidget()
        self.login_widget.setLayout(login_layout)