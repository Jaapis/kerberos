# AUTENTICAÇÃO KERBEROS USANDO SSH

Este projeto contempla a segunda nota da disciplina **ECOE10** lecionado no segundo semestre de 2018 na Universidade Federal de Itajubá e envolve as tecnologias **Kerberos** e **Git**.



## 1. Membros

- David dos Anjos Bovolenta - 25784
- Felipe Caetano Castilho - 26904
- Guilherme M. Brtoletto - 30076
- Rodrigo Toshiaki Horie - 26620



## 2. Preparação

#### *1. SUDO*

Foi utilizado o sistema operacional (SO) *Ubuntu 18.04* e o primeiro passo foi certificar que o pacote *sudo* estava instalado. Para isso utilizou-se o seguinte comando:

```bash
su -c 'apt-get install sudo'
```

Foi então apresentada uma mensagem pelo SO de que o pacote já estava instalado. O passo seguinte foi configurar *sudo* utilizando

```bash
su -c 'echo "USERNAME ALL=(ALL) ALL" >> /etc/sudoers'
```

Substituindo o USERNAME com o usuário do sistema.



#### *2. DEBCONF*

Diversos pacotes que serão utilizados irão pedir várias informações para o *debconf*, portanto configurou-se ele da seguinte maneira:

```bash
sudo dpkg-reconfigure debconf	
```

Coloca-se então, nas perguntas seguintes, `interface=Dialog` e `priority=low`.



#### *3. MANTENDO LOGS*

Para conseguir monitorar o procedimento, foi preciso utilzar um comando para pegar todas as rotinas e imprimir num arquivo:

```bash
cd /var/log; sudo tail -F daemon.log sulog user.log auth.log debug kern.log syslog dmesg messages kerberos/{krb5kdc,kadmin,krb5lib}.log
```



## 3. Kerberos 5

#### *1. INSTALAÇÃO*

Para instalar o *Kerberos* na máquina usou-se o comando:

```bash
sudo apt-get install krb5-{admin-server,kdc}
```

Algumas perguntas foram feitas pelo *Debconf*:

```
Default Kerberos version 5 realm? TESTE.COM

Add locations of default Kerberos servers to /etc/krb5.conf? Yes

Kerberos servers for your realm: teste.com

Administrative server for your Kerberos realm: teste.com

Create the Kerberos KDC configuration automatically? Yes

Run the Kerberos V5 administration daemon (kadmind)? Yes
```

E logo após a instalação o *kerberos-admin server* e o *kdc* tentarão iniciar, porém não há nenhum *realm* criado ainda.



#### *2. CONFIGURAÇÃO*

Para criar o server foi necessário utilizar:

```bash
sudo krb5_newrealm
```

E com isto o sistema pedirá uma senha mestre.

Foi, então, necessário configurar o arquivo `/etc/krb5.conf` da seguinte maneira:

```bash
.teste.com = TESTE.COM
teste.com = TESTE.COM
```

E no final do arquivo adicionou-se as linhas

```bash
[logging]
	kdc = FILE:/var/log/kerberos/krb5kdc.log
	admin_server = FILE:/var/log/kerberos/kadmin.log
	default = FILE:/var/log/kerberos/krb5lib.log
```

Para produzir arquivos de log.

Criou-se então o diretório de log e configurou-se as permissões necessárias.

```bash
sudo mkdir /var/log/kerberos
sudo touch /var/log/kerberos/{krb5kdc,kadmin,krb5lib}.log
sudo chmod -R 750  /var/log/kerberos
```

E, finalmente, para aplicar as mudanças no servidor do *kerberos* utilizou-se:

```bash
sudo invoke-rc.d krb5-admin-server restart
sudo invoke-rc.d krb5-kdc restart
```



#### *3. TESTE INICIAL*

Rodou-se:

```bash
sudo kadmin.local
Authenticating as principal admin/admin@teste.com with password.

kadmin.local: listprincs

K/M@TESTE.COM
kadmin/admin@TESTE.COM
kadmin/changepw@TESTE.COM
kadmin/krb1.test.com@TESTE.COM
krbtgt/teste.com@TESTE.COM

kadmin.local:  quit
```

E tudo ocorreu como esperado.



#### *4. PRINCPAL PRIVILEGIADO*

Adicionou-se o primeiro principal com privilégios.

```bash
sudo kadmin.local
Authenticating as principal admin/admin@STESTE.COM with password.

kadmin.local: addprinc -policy admin admin/admin

Enter password for principal "admin/admin@TESTE.COM": PASSWORD
Re-enter password for principal "admin/admin@TESTE.COM": PASSWORD
Principal "admin/admin@TESTE.COM" created.

kadmin.local: quit
```



#### *5. TESTE DO KADMIN*

Testou-se o principal criado anteriormente

```bash
sudo kadmin -p admin/admin
Authenticating as principal admin/admin@TESTE.COM with password.

Password for admin/admin@TESTE.COM: PASSWORD

kadmin: listprincs

K/M@TESTE.COM
admin/admin@TESTE.COM
kadmin/admin@TESTE.COM
kadmin/changepw@TESTE.COM
kadmin/history@TESTE.COM
kadmin/krb1.TESTE.COM@TESTE.COM
krbtgt/TESTE.COM@TESTE.COM

kadmin: quit
```



#### *6. PRINCIPAL NÃO-PRIVILEGIADO*

Foi, então, criado o principal sem privilégios.

```
sudo kadmin -p admin/admin
Authenticating as principal admin/admin@TESTE.COM with password.

Password for admin/admin@TESTE.COM: PASSWORD

kadmin:  addprinc -policy user monarch

Enter password for principal "krb@TESTE.COM": PASSWORD
Re-enter password for principal "krb@STESTE.COM": PASSWORD
Principal "krb@TESTE.COM" created.

kadmin:  quit
```



## 4. Autenticação

Para realizar a autenticação usa-se o comando `klist`. Inicialmente não há nenhuma credencial, portanto é necessário usar `kinit krb`.

No terminal cliente, então, usa-se o comando

```bash
ssh krb.teste.com
```

E se tudo está correto, a autenticação deve ocorrer com successo.

## 5. Funcionamento

Aqui está um vídoe demonstrando a funcionalidade do projeto: [Youtube](https://youtu.be/4fjoKbaJBCs)
