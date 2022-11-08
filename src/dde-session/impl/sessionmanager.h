#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

#include <QObject>
#include <QDBusObjectPath>
#include <QDBusInterface>
#include <QDBusContext>

#include "org_deepin_PowerManager1.h"
#include "org_deepin_daemon_Audio1.h"

#include "org_freedesktop_login1_Manager.h"
#include "org_freedesktop_login1_Session.h"
#include "org_freedesktop_login1_User.h"
#include "org_freedesktop_systemd1_Manager.h"
#include "org_freedesktop_DBus.h"

class SessionManager : public QObject, public QDBusContext
{
    Q_OBJECT
    Q_PROPERTY(bool Locked READ locked NOTIFY lockedChanged)
    Q_PROPERTY(int Stage READ stage NOTIFY stageChanged)
    Q_PROPERTY(QDBusObjectPath CurrentSessionPath READ currentSessionPath NOTIFY currentSessionPathChanged)
    Q_PROPERTY(QString CurrentUid READ currentUid NOTIFY currentUidChanged)

public:
    static SessionManager *instance();

    bool locked();
    QString currentUid();
    void setCurrentUid(QString uid);
    Q_DECL_DEPRECATED int stage();
    Q_DECL_DEPRECATED void setStage(int value);
    QDBusObjectPath currentSessionPath();
    void setCurrentSessionPath(QDBusObjectPath value);

    void init();
    void doLogout();

public Q_SLOTS:
    bool AllowSessionDaemonRun();
    bool CanHibernate();
    bool CanLogout();
    bool CanReboot();
    bool CanShutdown();
    bool CanSuspend();
    void ForceLogout();
    void ForceReboot();
    void ForceShutdown();
    Q_DECL_DEPRECATED void Logout();
    Q_DECL_DEPRECATED void PowerOffChoose();
    Q_DECL_DEPRECATED void Reboot();
    Q_DECL_DEPRECATED void Shutdown();
    bool Register(const QString &id);
    void RequestHibernate();
    void RequestLock();
    void RequestLogout();
    void RequestReboot();
    void RequestShutdown();
    void RequestSuspend();
    void SetLocked(bool lock);
    void ToggleDebug();

private:
    explicit SessionManager(QObject *parent = nullptr);

    void initConnections();
    void initSwapSched();
    void prepareLogout(bool force);
    void prepareShutdown(bool force);
    void clearCurrentTty();

    // tiny function
    void stopSogouIme();
    void stopLangSelector();
    void launchAutostopScripts();
    void startAtSpiService();
    void stopAtSpiService();
    void startObexService();
    void stopObexService();
    void stopPulseAudioService();
    void stopBAMFDaemon();
    void stopRedshift();
    void disconnectAudioDevices();

    void preparePlayShutdownSound();
    bool canPlayEvent(const QString &event);
    void playLogoutSound();
    void setDPMSMode(bool on);

    void handleOSSignal();

    void shutdown(bool force);
    void reboot(bool force);

private Q_SLOTS:
    void handleLoginSessionLocked();
    void handleLoginSessionUnlocked();

signals:
    // signals on dbus
    void Unlock();

    void lockedChanged(bool);
    void currentUidChanged(QString);
    void stageChanged(int);
    void currentSessionPathChanged(QDBusObjectPath);

private:
    bool m_locked;
    QString m_currentUid;
    int m_stage;
    QDBusObjectPath m_currentSessionPath;

    // dbus client
    org::deepin::daemon::PowerManager1 *m_powerInter;
    org::deepin::daemon::Audio1 *m_audioInter;
    org::freedesktop::login1::Manager *m_login1ManagerInter;
    org::freedesktop::login1::User *m_login1UserInter;
    org::freedesktop::login1::Session *m_login1SessionInter;
    org::freedesktop::systemd1::Manager *m_systemd1ManagerInter;
    org::freedesktop::DBus *m_DBusInter;
};

#endif // SESSIONMANAGER_H
