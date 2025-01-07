#include <QApplication>
#include <QWidget>
#include <QRegion>
#include <QPainter>
#include <QCamera>
//#include <QCameraPermission>
#include <QPermissions>
#include <QVideoWidget>
#include <QMediaDevices>
#include <QMediaCaptureSession>
//#include <QCameraInfo>  // Add this line
//#include <QCameraViewfinder>
#include <iostream>
#include <QVBoxLayout>
#include <QCommandLineParser>

class CircularWindow : public QWidget {
public:
    CircularWindow(int cameraIndex, QWidget *parent = nullptr) : QWidget(parent), cameraIndex(cameraIndex) {
        setWindowTitle("Circular Window");
        setWindowFlags(Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);  // Remove window frame

        // Set the circular shape for the initial window size
        setCircularMask();

        // Set up the camera and viewfinder
        setupCamera();

        // Set up the layout
      /*  QVBoxLayout *layout = new QVBoxLayout(this);
        layout->addWidget(viewfinder);

        setLayout(layout);*/

        // Start the camera
        camera->start();
    }

protected:
    void paintEvent(QPaintEvent *event) override {
        Q_UNUSED(event);
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing, true);

        // Draw a simple circle in the window
        painter.setBrush(Qt::blue);
        painter.drawEllipse(rect());
    }

    void resizeEvent(QResizeEvent *event) override {
        Q_UNUSED(event);
        // Update the circular mask when the window is resized
        setCircularMask();
        const int offset = 100;
        viewfinder->setGeometry(-offset, -offset, width() + (offset*2), height() + (offset*2));

    }

private:
    void setCircularMask() {
        // Create a circular region using the entire width and height of the window
        QRegion circularRegion(0, 0, width(), height(), QRegion::Ellipse);

        // Set the circular region as the mask for the window
        setMask(circularRegion);
    }

    void setupCamera() {
        // Create a camera object

        for (auto i:QMediaDevices::videoInputs())
        {
            std::cout << i.description().toStdString() << std::endl;
        }

        //QPermission::requestPermission
        //QCameraPermission microphonePermission;
        //switch (qApp->checkPermission(microphonePermission)) {
        /*case Qt::PermissionStatus::Undetermined:
            qApp->requestPermission(microphonePermission, this,
                        &VoiceMemoWidget::onRecordingInitiated);
            return;
        case Qt::PermissionStatus::Denied:
            m_permissionInstructionsDialog->show();
            return;
        case Qt::PermissionStatus::Granted:
            m_microphone->startRecording();*/
        //}

        camera = new QCamera(QMediaDevices::defaultVideoInput());

        // Check the available cameras
        /*QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
        if (cameraIndex >= 0 && cameraIndex < cameras.size()) {
            camera->setCamera(cameras.at(cameraIndex));
        } else {
            qWarning() << "Invalid camera index. Using the default camera.";
        }*/

        // Create a viewfinder widget
        viewfinder = new QVideoWidget(this);

        // Set the size policy to ignored to achieve the zoom effect
        viewfinder->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

        // Set the camera to use the viewfinder
        //camera->setViewfinder(viewfinder);

        //QCamera camera(QMediaDevices::defaultVideoInput());
        //camera.start();

         mediaCaptureSession = new QMediaCaptureSession(this);
        mediaCaptureSession->setCamera(camera);
        mediaCaptureSession->setVideoOutput(viewfinder);
    }

    QCamera *camera;
    QVideoWidget *viewfinder;
    QMediaCaptureSession *mediaCaptureSession;
    int cameraIndex;
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    app.requestPermission(QCameraPermission{}, [] (const QPermission &permission) {
        std::cout << "Permission granted " << (permission.status() == Qt::PermissionStatus::Granted) << std::endl;
    });

    // Set up command-line parser
    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addOption({{"c", "camera"}, "Select camera by index (default: 0)", "index", "0"});

    // Process the command-line arguments
    parser.process(app);

    // Get the camera index from the command line
    int cameraIndex = parser.value("camera").toInt();

    CircularWindow window(cameraIndex);
    window.resize(240, 240);  // Adjust the initial window size as needed
    window.show();

    return app.exec();
}
