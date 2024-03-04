#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPlainTextEdit> // Incluir la clase QPlainTextEdit para mostrar la información de las neuronas

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_Principio_clicked()
{
    // Obtener los datos de la neurona desde los campos de texto
    QString id = ui->data_ID->text();
    double voltaje = ui->data_VOLTAJE->text().toDouble();
    int posX = ui->POSICIONX->text().toInt();
    int posY = ui->POSICIONY->text().toInt();
    int red = ui->RED->text().toInt();
    int green = ui->GREEN->text().toInt();
    int blue = ui->BLUE->text().toInt();

    // Crear una nueva neurona con los datos proporcionados
    Neurona neurona(id, voltaje, posX, posY, red, green, blue);

    // Agregar la neurona al principio de la lista en la clase administradora
    administradora.agregarInicio(neurona);

    ui->RESULTADO->setPlainText("Se guardó la neurona al principio.");
}

void MainWindow::on_Final_clicked()
{
    // Obtener los datos de la neurona desde los campos de texto
    QString id = ui->data_ID->text();
    double voltaje = ui->data_VOLTAJE->text().toDouble();
    int posX = ui->POSICIONX->text().toInt();
    int posY = ui->POSICIONY->text().toInt();
    int red = ui->RED->text().toInt();
    int green = ui->GREEN->text().toInt();
    int blue = ui->BLUE->text().toInt();

    // Crear una nueva neurona con los datos proporcionados
    Neurona neurona(id, voltaje, posX, posY, red, green, blue);

    // Agregar la neurona al final de la lista en la clase administradora
    administradora.agregarFinal(neurona);

    ui->RESULTADO->setPlainText("Se guardó la neurona al final.");
}

void MainWindow::on_Muestra_clicked()
{
    // Obtener la información de todas las neuronas en la clase administradora
    std::vector<Neurona> neuronas = administradora.obtenerNeuronas();

    // Mostrar la información de todas las neuronas en el QPlainTextEdit
    QString texto;
    for(const auto& neurona : neuronas) {
        texto += "ID: " + neurona.getID() +"\n";
        texto += "Voltaje: " + QString::number(neurona.getVoltaje()) + "\n";
        texto += "Posicion en X: " + QString::number(neurona.getPosX()) + "\n";
        texto += "Posicion en Y: " + QString::number(neurona.getPosY()) + "\n";
        texto += "Red: " + QString::number(neurona.getRed()) + "\n";
        texto += "Green : " + QString::number(neurona.getGreen()) + "\n";
        texto += "Blue : " + QString::number(neurona.getBlue()) + "\n";
        texto += "\n";

        // Agregar más información según los campos de la neurona
    }
    ui->RESULTADO->setPlainText(texto);


}

void MainWindow::on_Guardar_clicked()
{
    // Obtener la información de todas las neuronas en la clase administradora
    std::vector<Neurona> neuronas = administradora.obtenerNeuronas();

    QString texto;
    for(const auto& neurona : neuronas) {
        texto += neurona.getID() + ","; // Separar los datos por comas
        texto += QString::number(neurona.getVoltaje()) + ",";
        texto += QString::number(neurona.getPosX()) + ",";
        texto += QString::number(neurona.getPosY()) + ",";
        texto += QString::number(neurona.getRed()) + ",";
        texto += QString::number(neurona.getGreen()) + ",";
        texto += QString::number(neurona.getBlue());
        texto += "\n"; // Agregar nueva línea al final de cada neurona
    }

    QString rutaArchivo = QFileDialog::getSaveFileName(this, tr("Guardar archivo"), "C:/Users/josue/OneDrive/Documentos/pruebas", tr("Archivos de texto (*.txt);;Todos los archivos (*)"));
    if (!rutaArchivo.isEmpty()) {
        QFile archivo(rutaArchivo);
        if (archivo.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&archivo);
            out << texto;
            archivo.close();
        } else {
            ui->RESULTADO->setPlainText("");
            // Manejar errores al abrir el archivo
        }
    }
}


void MainWindow::on_Recuperar_clicked()
{
    QString rutaArchivo = QFileDialog::getOpenFileName(this, tr("Abrir archivo"), "C:/Users/josue/OneDrive/Documentos/pruebas", tr("Archivos de texto (*.txt);;Todos los archivos (*)"));
    if (!rutaArchivo.isEmpty()) {
        QFile archivo(rutaArchivo);
        if (archivo.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&archivo);
            QString contenido = in.readAll();
            ui->RESULTADO->setPlainText(contenido);
            archivo.close();
        } else {
            qDebug() << "Error al abrir el archivo para lectura:" << archivo.errorString();
            // Manejar errores al abrir el archivo
        }
    }
}




void MainWindow::on_Buscar_clicked()
{



    // Obtener el ID de la neurona a buscar desde el QLineEdit
    QString idABuscar = ui->busqueda->text();

    // Obtener todas las neuronas del archivo
    std::vector<Neurona> neuronas;

    QString rutaArchivo = "C:/Users/josue/OneDrive/Documentos/pruebas/4.txt"; // Reemplaza con la ruta correcta
    QFile archivo(rutaArchivo);
    if (archivo.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&archivo);
        while (!in.atEnd()) {
            QString linea = in.readLine(); // Leer una línea del archivo
            QStringList datos = linea.split(","); // Separar los datos por comas
            if (datos.size() == 7) {
                Neurona neurona(datos[0], datos[1].toDouble(), datos[2].toInt(), datos[3].toInt(), datos[4].toInt(), datos[5].toInt(), datos[6].toInt());
                neuronas.push_back(neurona);
            }
        }
        archivo.close();
    } else {
        qDebug() << "Error al abrir el archivo para lectura:" << archivo.errorString();
        // Manejar errores al abrir el archivo
        return;
    }

    // Realizar búsqueda lineal de las neuronas por su ID
    std::vector<Neurona> neuronasEncontradas;

    for(const auto& neurona : neuronas) {
        if (neurona.getID() == idABuscar) {
            neuronasEncontradas.push_back(neurona);
        }
    }

    // Mostrar las neuronas encontradas en el QTableWidget
    if (!neuronasEncontradas.empty()) {
        ui->RESULTADO2->clear();
        ui->RESULTADO2->setRowCount(neuronasEncontradas.size());
        for (int i = 0; i < neuronasEncontradas.size(); ++i) {
            const Neurona& neurona = neuronasEncontradas[i];
            ui->RESULTADO2->setItem(i, 0, new QTableWidgetItem(neurona.getID()));
            ui->RESULTADO2->setItem(i, 1, new QTableWidgetItem(QString::number(neurona.getVoltaje())));
            ui->RESULTADO2->setItem(i, 2, new QTableWidgetItem(QString::number(neurona.getPosX())));
            ui->RESULTADO2->setItem(i, 3, new QTableWidgetItem(QString::number(neurona.getPosY())));
            ui->RESULTADO2->setItem(i, 4, new QTableWidgetItem(QString::number(neurona.getRed())));
            ui->RESULTADO2->setItem(i, 5, new QTableWidgetItem(QString::number(neurona.getGreen())));
            ui->RESULTADO2->setItem(i, 6, new QTableWidgetItem(QString::number(neurona.getBlue())));
        }
    } else {
        ui->RESULTADO2->clear();
        ui->RESULTADO2->setItem(0, 0, new QTableWidgetItem("ID no encontrado"));
        // No se encontraron neuronas con el ID buscado, puedes mostrar un mensaje o realizar alguna acción
    }



}

