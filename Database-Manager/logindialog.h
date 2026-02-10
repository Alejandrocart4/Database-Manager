#pragma once
#include <QDialog>

class QComboBox;
class QLineEdit;
class QSpinBox;
class QPushButton;
class QLabel;
class QCheckBox;

struct ConnectionProfile
{
    QString name;     // "Local MariaDB",
    QString host;     // "localhost"
    int     port;     // 3306
    QString database; // puede quedar vacío
    QString user;
    QString driver;   // "MariaDB ODBC 3.2 Driver"
};

class LoginDialog : public QDialog
{
    Q_OBJECT
public:
    explicit LoginDialog(QWidget* parent = nullptr);

    QString dsn() const;                // DSN final para DbSession::openWithDsn
    ConnectionProfile profile() const;  // datos actuales del formulario

private:
    void buildUi();
    void loadProfiles();
    void saveProfiles();
    void applyProfileToUi(const ConnectionProfile& p);
    ConnectionProfile profileFromUi() const;

private slots:
    void onProfileChanged(int idx);
    void onSaveProfile();
    void onDeleteProfile();

private:
    // UI
    QComboBox*  m_profiles = nullptr;

    QLineEdit*  m_name = nullptr;
    QLineEdit*  m_host = nullptr;
    QSpinBox*   m_port = nullptr;
    QLineEdit*  m_db = nullptr;
    QLineEdit*  m_user = nullptr;
    QLineEdit*  m_pass = nullptr;

    QPushButton* m_btnSaveProfile = nullptr;
    QPushButton* m_btnDeleteProfile = nullptr;

    QPushButton* m_btnConnect = nullptr;
    QPushButton* m_btnCancel = nullptr;

    // data
    QVector<ConnectionProfile> m_items;
};
