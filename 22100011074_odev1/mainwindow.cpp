// SUDE ÖZSOY 22100011074
/*  -Ek özellik olarak süreyi 3 saniye artırması için 3(simge) butonu kullandım. Tıklandığında süre artacak.
    !-Skorhesapla() fonksiyonundaki dosyaya veri yazmada resourcesdaki desyaları değiştirmeme izin vermediği
    için dosyanın bilgisayarımdaki konumunu yolladım. Başka bilgisayarda çalıştırırken dikkat edilmeli*/

#include "mainwindow.h"
#include "ui_mainwindow.h"
int toplamkarpuz = 0;
int tiklanan = 0,tiklanmayan=0,eksure=0;
int sure=30;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QPixmap resim(":/images/back.jpg");                 //Arkaplandaki fotoğrafı eklemek için
    QLabel *arkaplan = new QLabel(this);
    arkaplan->setPixmap(resim);
    arkaplan->setGeometry(0, 0, 1350, 750);

    Label = new QLabel(this);                           //En üstteki yazıların arkasındaki beyaz arkaplanı oluşturur
    Label->setAlignment(Qt::AlignLeft);
    Label->setStyleSheet("background-color: white;font-size: 16px; font-weight: bold;");
    Label->setGeometry(0, 0, 1350, 100);
    Label->show();

    LabelSure = new QLabel(this);                       //Surenin yazdığı labeli oluşturur
    LabelSure->setAlignment(Qt::AlignCenter);
    LabelSure->setStyleSheet("font-size: 25px; font-weight: bold;");
    LabelSure->setGeometry(0,0,100,100);

    LabelKesilen = new QLabel(this);                    //Kesilen karpuz sayısının yazdığı labeli oluşturur
    LabelKesilen->setAlignment(Qt::AlignRight);
    LabelKesilen->setStyleSheet("font-size: 25px; font-weight: bold;");
    LabelKesilen->setGeometry(1012,8,250,100);

    LabelKesilmeyen = new QLabel(this);                 //Kesilmeyen karpuz sayısının yazdığı labeli oluşturur
    LabelKesilmeyen->setAlignment(Qt::AlignRight);
    LabelKesilmeyen->setStyleSheet("font-size: 25px; font-weight: bold;");
    LabelKesilmeyen->setGeometry(1012,40,250,100);

    QStringList konumliste;
    konumliste = DosyaOkuma();              //Dosyadan hazır konumları okur listeye atar

    timer30 = new QTimer(this);
    timer30->start(1000);                   //Oyun toplam süresi için timer
    connect(timer30, &QTimer::timeout, this, [=]() {
        LabelSure->setText("Sure: <html><font color='blue'>"+QString::number(sure)+"</font>");                  //Ekrana bilgileri yazdırma
        LabelSure->show();

        LabelKesilen->setText("Kesilen Karpuz: <html><font color='red'>"+QString::number(tiklanan)+"</font>");
        LabelKesilen->show();

        LabelKesilmeyen->setText("Kaçırılan Karpuz: <html><font color='red'>"+QString::number(tiklanmayan)+"</font>");
        LabelKesilmeyen->show();
        SureAzalt();                        //30 saniye bitene kadar 1 snde bir sureyi azaltır.
    });
    timer2=new QTimer(this);                //yarım saniyede bir karpuz oluşturmak için açıldı
    timer2->start(500);
    connect(timer2, &QTimer::timeout, this, [=]() {
        KarpuzOlustur(konumliste);
    });

    timerEk3=new QTimer(this);              //5 saniyede bir ek sure butonu oluşturmak için açıldı
    timerEk3->start(5000);
    connect(timerEk3, &QTimer::timeout, this, [=]() {
        EkSureOlustur(konumliste);
    });

}

void MainWindow::SureAzalt()
{
    if(sure == 0)                           //sure bitince
    {
        timer30->stop();
        timer2->stop();
        timerEk3->stop();
        SkorHesapla();                      //skorları hesaplar, ekrana ve dosyaya yazdırır.
        this->close();                      //program sonlanır
    }
    else if(sure <= 10)                     //sure 10 saniyeden düşmeye başlayınca kum saati simgesi göstermesi için
    {
        QPushButton *kumsaati = new QPushButton(this);
        QIcon *icon = new QIcon();
        icon->addPixmap(QPixmap(":/images/kumsaati.png"),QIcon::Normal,QIcon::Off);
        kumsaati->setIcon(*icon);
        kumsaati->setIconSize(QSize(50,50));
        kumsaati->setStyleSheet("Background-color:transparent;border:none;");
        kumsaati->setGeometry(rand()%400,rand()%300,70,70);
        kumsaati->move(100,25);
        kumsaati->show();
    }
    sure--;                                  //sureyi tek tek azaltır
}

void MainWindow::SkorHesapla()
{
    QStringList liste;
    QString dosyaYolu = ":/images/skorlar.txt";

    QFile dosya(dosyaYolu);
    if (!dosya.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Dosya açılamadı!";
    }

    QTextStream okuyucu(&dosya);
    QString satir;
    while (!okuyucu.atEnd()) {
        satir = okuyucu.readLine();
        liste.append(satir);                    //dosyadan okunan verileri satir satir listeye atar
    }
    dosya.close();
    int enbuyuk=0;
    for(int i=0;i<liste.size();i++)             //listedeki en büyük sayıyı bulması için
    {
        if(enbuyuk < liste[i].toInt())
        {
            enbuyuk = liste[i].toInt();
        }
    }
    if(enbuyuk >= tiklanan)                     //kullanıcının skorunu maksimum skorla karşılaştırı ona göre bilgi ekranı verir
    {
        QMessageBox::information(this,"BİLGİ!","Oyun Bitti! Maksimum Skoru Geçemediniz!\n Kesilen Karpuz Sayısı:"+QString::number(tiklanan)
                                +"\n Kaçırılan Karpuz Sayısı: "+QString::number(tiklanmayan)+"\n Kullanılan Ek Süre:"+QString::number(eksure)
                                +"\n Maksimum Skor:"+QString::number(enbuyuk),"OK");
    }
    else
        QMessageBox::information(this,"BİLGİ!","Oyun Bitti! Tebrikler Maksimum Skor Sizde!\n Kesilen Karpuz Sayısı:"+QString::number(tiklanan)
                                +"\n Kaçırılan Karpuz Sayısı: "+QString::number(tiklanmayan)+"\n Kullanılan Ek Süre:"+QString::number(eksure)
                                +"\n Maksimum Skor:"+QString::number(enbuyuk),"OK");
    dosyaYolu = "C:/QT PROJECT/22100011074_odev1/skorlar.txt";  //resourcesdeki dosya yolunu ekleyince üzerine yazı eklemeye izin vermiyor bu yüzden direkt dosya yolunu ekledim.
    QFile file(dosyaYolu);

    if (file.open(QIODevice::Append | QIODevice::Text)) {       //dosyaya oynanan oyundaki skoru yazdırır
        QTextStream out(&file);
        out << QString::number(tiklanan) + "\n";
        file.close();
        qDebug() << "Veri dosyaya eklendi.";
    } else {
        qDebug() << "Dosya açılamadı";
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

QStringList MainWindow::DosyaOkuma()     //konumlar dosyasını okuması için
{
    QStringList liste;
    QString dosyaYolu = ":/images/konumlar.txt";

    QFile dosya(dosyaYolu);
    if (!dosya.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Dosya açılamadı!";
    }

    QTextStream okuyucu(&dosya);
    QString satir;
    while (!okuyucu.atEnd()) {
        satir = okuyucu.readLine();
        liste.append(satir);
    }
    dosya.close();
    return liste;
}

void MainWindow::KarpuzOlustur(QStringList konumliste)
{
    int indis = QRandomGenerator::global()->bounded(0,konumliste.size());    //konumlar listesinin indis boyutu aralığında random sayı oluşturur
    QStringList konum = konumliste[indis].split(" ");                   //listedeki seçilen konumu x ve y koordinatı olarak ayırmak için split kullandım
    int x = konum[0].toInt();
    int y = konum[1].toInt();

    QPushButton *karpuz = new QPushButton(this);                        //karpuz için button oluşturup düzenlemelerini yaptım
    QIcon *icon = new QIcon();
    icon->addPixmap(QPixmap(":/images/1.png"),QIcon::Normal,QIcon::Off);
    karpuz->setIcon(*icon);
    karpuz->setIconSize(QSize(70,70));
    karpuz->setStyleSheet("Background-color:transparent;border:none;");
    karpuz->setGeometry(rand()%400,rand()%300,70,70);
    karpuz->move(x,y);                  //listeden random seçilen koordinatlara yerleştirdim.
    karpuz->show();

    toplamkarpuz++;                     //bir yerde kullanmadım ama toplam karpuz sayısını da hesapladım

    karpuz->setProperty("clicked", false);      //karpuza clicked adında özellik ekledim ve ilk durumda bunu false yaptım

    QTimer *timer = new QTimer(this);
    timer->start(15);                           //karpuzların aşağı doğru kayması için timer açtım
    connect(timer, &QTimer::timeout, [=]() {
        if (!karpuz->property("clicked").toBool())      //eğer karpuzların clicked özelliği hala false'sa karpuzlar aşağı kaymaya devam edecek
        {
            karpuz->move(karpuz->x(), karpuz->y() +3);
            if(karpuz->y() > 850)                       //karpuzun konumu tıklanmadan ekrandan çıkmışsa kesilmeyen karpuzlara ekledim
            {
                tiklanmayan++;
                timer->stop();                          //aşağı kaymayı durdurmak için timeri durdurdum
            }
        }
    });

    QTimer *timer3 = new QTimer(this);
    timer3->setSingleShot(true);
    connect(timer3, &QTimer::timeout, [=]() {
        karpuz->setStyleSheet("background-color: rgba(0, 0, 0, 0);");       //Tıklanmış olan butonu şeffaf yaptım
        karpuz->setEnabled(false);                                          //Pasif hale getirdim
        karpuz->setIcon(QIcon());
    });

    connect(karpuz, &QPushButton::clicked, this, [=]() {
        karpuz->setProperty("clicked", true);                       //karpuza tıklandığında karpuzun clicked özelliğini true yaptım
        karpuzTiklandi();
        timer3->start(1500);                                        //karpuz tıklandığında 1.5 saniye ekranda kalıp silinmesi için timer
    });
}

void MainWindow::EkSureOlustur(QStringList konumliste)
{
    int indis = QRandomGenerator::global()->bounded(0,konumliste.size());
    QStringList konum = konumliste[indis].split(" ");
    int x = konum[0].toInt();
    int y = konum[1].toInt();

    QPushButton *ucsn = new QPushButton(this);
    QIcon *ekicon = new QIcon();
    ekicon->addPixmap(QPixmap(":/images/3sn.png"),QIcon::Normal,QIcon::Off);
    ucsn->setIcon(*ekicon);
    ucsn->setIconSize(QSize(70,70));
    ucsn->setStyleSheet("Background-color:transparent;border:none;");
    ucsn->setGeometry(rand()%400,rand()%300,70,70);
    ucsn->move(x,y);
    ucsn->show();

    eksure+=3;                              //Toplam kullanılan ek süreyi hesapladım.

    ucsn->setProperty("clicked", false);

    QTimer *timerEk = new QTimer(this);
    timerEk->start(10);
    connect(timerEk, &QTimer::timeout, [=]() {
        if (!ucsn->property("clicked").toBool())
        {
            ucsn->move(ucsn->x(), ucsn->y() +3);
            if(ucsn->y() > 850)
                timerEk->stop();
        }
    });

    connect(ucsn, &QPushButton::clicked, this, [=]() {
        ucsn->setProperty("clicked", true);
        ucsn->setStyleSheet("background-color: rgba(0, 0, 0, 0);"); // Butonu şeffaf yap
        ucsn->setEnabled(false); // Butonu pasif hale getir
        ucsn->setIcon(QIcon());
        EkSureTiklandi();
    });
}

void MainWindow::karpuzTiklandi()
{
    QPushButton *karpuz = qobject_cast<QPushButton*>(sender());
    if (karpuz) {
        QIcon icon(":/images/2.png");      //Karpuza tıklandığında diğer karpuz simgesini koymsı için
        karpuz->setIcon(icon);
        tiklanan++;                        //Kesilen karpuz sayısını artırır
    }
}

void MainWindow::EkSureTiklandi()
{
    LabelEk = new QLabel(this);
    LabelEk->setAlignment(Qt::AlignCenter);
    LabelEk->setStyleSheet("font-size: 25px; font-weight: bold;font-style: italic; color: purple;");
    LabelEk->setGeometry(110,0,250,100);
    LabelEk->setText("+3 Ek Sure");             //ek süre eklendiğinde sürenin yanında ek süre eklendiğine dair bilgilendirme
    LabelEk->show();
    QTimer::singleShot(2000, [=]() {            //bilgilendirme mesajı 2 sn ekranda kalması için
        LabelEk->setText("");
    });
    sure+=3;                                    //sureyi artırır.
}
