from PyQt5.QtWidgets import QWidget, QLabel, QLineEdit, QVBoxLayout, QPushButton, QStackedWidget


class MainWindow(QWidget):
    def __init__(self):
        super().__init__()
        self.initUI()

    def initUI(self):
        login_scene = self.login_scene()
        # Create button for the second scene
        back_button = QPushButton('Back to First Scene', self)
        back_button.clicked.connect(self.switch_scene)

        # Create layout for the second scene
        layout2 = QVBoxLayout()
        layout2.addWidget(back_button)

        # Create widget for the second scene
        self.second_scene_widget = QWidget()
        self.second_scene_widget.setLayout(layout2)

        # Create stacked widget to manage different scenes
        self.stacked_widget = QStackedWidget(self)
        self.stacked_widget.addWidget(login_scene)
        self.stacked_widget.addWidget(self.second_scene_widget)

        # Set up the main layout
        main_layout = QVBoxLayout(self)
        main_layout.addWidget(self.stacked_widget)

        self.setGeometry(300, 300, 300, 200)
        self.setWindowTitle('PyQt5 StackedWidget Example')

    def login_scene(self):
        # Create labels and text fields for the first scene
        label1 = QLabel('Text Field 1:')
        text_field1 = QLineEdit(self)

        label2 = QLabel('Text Field 2:')
        text_field2 = QLineEdit(self)

        label3 = QLabel('Text Field 3:')
        text_field3 = QLineEdit(self)

        # Create button to switch to the next scene
        switch_button = QPushButton('Login', self)
        switch_button.clicked.connect(self.switch_scene)

        # Create layout for the first scene
        login_layout = QVBoxLayout()
        login_layout.addWidget(label1)
        login_layout.addWidget(text_field1)
        login_layout.addWidget(label2)
        login_layout.addWidget(text_field2)
        login_layout.addWidget(label3)
        login_layout.addWidget(text_field3)
        login_layout.addWidget(switch_button)

        # Create widget for the first scene
        login_widget = QWidget()
        login_widget.setLayout(login_layout)
        return login_widget

    def switch_scene(self):
        current_index = self.stacked_widget.currentIndex()
        new_index = (current_index + 1) % self.stacked_widget.count()
        self.stacked_widget.setCurrentIndex(new_index)
