#include "LoginDialog.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QComboBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>
#include <QSettings>

static QString profilesKey() { return "profiles"; }

LoginDialog::LoginDialog(QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle("Inicio de sesión (MariaDB)");
    setModal(true);
    buildUi();
    loadProfiles();
}

void LoginDialog::buildUi()
{
    m_profiles = new QComboBox;

    m_name = new QLineEdit;
    m_host = new QLineEdit("localhost");
    m_port = new QSpinBox;
    m_port->setRange(1, 65535);
    m_port->setValue(3306);

    m_db   = new QLineEdit;     // opcional
    m_user = new QLineEdit;
    m_pass = new QLineEdit;
    m_pass->setEchoMode(QLineEdit::Password);

    m_btnSaveProfile = new QPushButton("Guardar perfil");
    m_btnDeleteProfile = new QPushButton("Eliminar perfil");

    m_btnConnect = new QPushButton("Conectar");
    m_btnCancel  = new QPushButton("Cancelar");

    auto* topRow = new QHBoxLayout;
    topRow->addWidget(new QLabel("Perfiles:"));
    topRow->addWidget(m_profiles, 1);
    topRow->addWidget(m_btnSaveProfile);
    topRow->addWidget(m_btnDeleteProfile);

    auto* form = new QFormLayout;
    form->addRow("Nombre perfil:", m_name);
    form->addRow("Host:", m_host);
    form->addRow("Puerto:", m_port);
    form->addRow("Database (opcional):", m_db);
    form->addRow("Usuario:", m_user);
    form->addRow("Contraseña:", m_pass);

    auto* btnRow = new QHBoxLayout;
    btnRow->addStretch(1);
    btnRow->addWidget(m_btnConnect);
    btnRow->addWidget(m_btnCancel);

    auto* root = new QVBoxLayout(this);
    root->addLayout(topRow);
    root->addLayout(form);
    root->addLayout(btnRow);

    connect(m_profiles, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &LoginDialog::onProfileChanged);

    connect(m_btnSaveProfile, &QPushButton::clicked,
            this, &LoginDialog::onSaveProfile);

    connect(m_btnDeleteProfile, &QPushButton::clicked,
            this, &LoginDialog::onDeleteProfile);

    connect(m_btnConnect, &QPushButton::clicked, this, [this](){
        // Validaciones mínimas
        if (m_host->text().trimmed().isEmpty() || m_user->text().trimmed().isEmpty()) {
            QMessageBox::warning(this, "Faltan datos", "Host y usuario son obligatorios.");
            return;
        }
        accept();
    });

    connect(m_btnCancel, &QPushButton::clicked, this, &QDialog::reject);
}

void LoginDialog::loadProfiles()
{
    // Guardamos en QSettings (INI) dentro del sistema del usuario.
    // Esto cumple “almacenar múltiples conexiones” sin depender de tablas.
    QSettings s("UNITEC", "Database-Manager");

    m_items.clear();
    m_profiles->clear();

    const int n = s.beginReadArray(profilesKey());
    for (int i = 0; i < n; ++i) {
        s.setArrayIndex(i);
        ConnectionProfile p;
        p.name     = s.value("name").toString();
        p.host     = s.value("host", "localhost").toString();
        p.port     = s.value("port", 3306).toInt();
        p.database = s.value("database").toString();
        p.user     = s.value("user").toString();
        p.driver   = s.value("driver", "MariaDB ODBC 3.2 Driver").toString();

        if (!p.name.trimmed().isEmpty())
            m_items.push_back(p);
    }
    s.endArray();

    // Si no hay perfiles, crea uno base
    if (m_items.isEmpty()) {
        ConnectionProfile p;
        p.name = "Local MariaDB";
        p.host = "localhost";
        p.port = 3306;
        p.database = "";
        p.user = "root";
        p.driver = "MariaDB ODBC 3.2 Driver";
        m_items.push_back(p);
        saveProfiles();
    }

    for (const auto& p : m_items)
        m_profiles->addItem(p.name);

    // aplicar el primero
    if (!m_items.isEmpty())
        applyProfileToUi(m_items[0]);
}

void LoginDialog::saveProfiles()
{
    QSettings s("UNITEC", "Database-Manager");
    s.remove(profilesKey()); // limpia
    s.beginWriteArray(profilesKey(), m_items.size());
    for (int i = 0; i < m_items.size(); ++i) {
        s.setArrayIndex(i);
        s.setValue("name", m_items[i].name);
        s.setValue("host", m_items[i].host);
        s.setValue("port", m_items[i].port);
        s.setValue("database", m_items[i].database);
        s.setValue("user", m_items[i].user);
        s.setValue("driver", m_items[i].driver);
    }
    s.endArray();
}

void LoginDialog::applyProfileToUi(const ConnectionProfile& p)
{
    m_name->setText(p.name);
    m_host->setText(p.host);
    m_port->setValue(p.port);
    m_db->setText(p.database);
    m_user->setText(p.user);
    // contraseña nunca se guarda
    m_pass->clear();
}

ConnectionProfile LoginDialog::profileFromUi() const
{
    ConnectionProfile p;
    p.name = m_name->text().trimmed();
    p.host = m_host->text().trimmed();
    p.port = m_port->value();
    p.database = m_db->text().trimmed();
    p.user = m_user->text().trimmed();
    p.driver = "MariaDB ODBC 3.2 Driver";
    return p;
}

void LoginDialog::onProfileChanged(int idx)
{
    if (idx < 0 || idx >= m_items.size()) return;
    applyProfileToUi(m_items[idx]);
}

void LoginDialog::onSaveProfile()
{
    ConnectionProfile p = profileFromUi();
    if (p.name.isEmpty()) {
        QMessageBox::warning(this, "Perfil", "El nombre del perfil es obligatorio.");
        return;
    }

    // Actualiza si ya existe por nombre, si no agrega
    int found = -1;
    for (int i = 0; i < m_items.size(); ++i) {
        if (m_items[i].name.compare(p.name, Qt::CaseInsensitive) == 0) {
            found = i;
            break;
        }
    }

    if (found >= 0) {
        m_items[found] = p;
        m_profiles->setItemText(found, p.name);
        m_profiles->setCurrentIndex(found);
    } else {
        m_items.push_back(p);
        m_profiles->addItem(p.name);
        m_profiles->setCurrentIndex(m_items.size() - 1);
    }

    saveProfiles();
    QMessageBox::information(this, "Perfil", "Perfil guardado.");
}

void LoginDialog::onDeleteProfile()
{
    const int idx = m_profiles->currentIndex();
    if (idx < 0 || idx >= m_items.size()) return;

    if (m_items.size() == 1) {
        QMessageBox::warning(this, "Perfil", "Debes tener al menos un perfil.");
        return;
    }

    m_items.removeAt(idx);
    m_profiles->removeItem(idx);
    saveProfiles();

    if (!m_items.isEmpty())
        applyProfileToUi(m_items[m_profiles->currentIndex()]);
}

QString LoginDialog::dsn() const
{
    const QString driver = "MariaDB ODBC 3.2 Driver";
    const QString host = m_host->text().trimmed();
    const int port = m_port->value();
    const QString database = m_db->text().trimmed();
    const QString user = m_user->text().trimmed();
    const QString pass = m_pass->text(); // no trimmed para permitir espacios si existieran

    // DATABASE puede quedar vacío; algunos drivers lo aceptan
    QString dsn = QString("DRIVER={%1};SERVER=%2;PORT=%3;UID=%4;PWD=%5;")
                      .arg(driver, host)
                      .arg(port)
                      .arg(user, pass);

    if (!database.isEmpty())
        dsn += QString("DATABASE=%1;").arg(database);

    return dsn;
}

ConnectionProfile LoginDialog::profile() const
{
    return profileFromUi();
}
