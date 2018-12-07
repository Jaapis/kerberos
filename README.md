#Configurar o Kerberos
#Nesta configura��o utiliza-se 3 maquinas diferentes. Um cliente, um provedor de servi�o e um distribuidor de chaves. As tres devem estar rodando o sistema operacional ubuntu. Para este caso utilizou-se tres maquinas virtuais diferentes com a placa de rede configurada em Bridge.

#mudar o nome da maquina kerberos kdc. Fazer o mesmo para as tres maquinas
#Na maquina kdc

sudo hostnamectl set-hostname kdc.fcc.com 

#Na maquina do servidor de servi�o

sudo hostnamectl set-hostname servidor.fcc.com 

#Na maquina cliente

sudo hostnamectl set-hostname client.fcc.com

#O dominio "fcc.com" � usado somente para exemplifica��o

#mudar dominios de hosts em todas as maquinas
sudo gedit /etc/host

#adicionar os ips das maquinas utilizadas
#Substituir os ips pelos ips das maquinas virtuais (ifconfig para visualizar)

192.168.0.108	kdc.fcc.com	kdc
192.168.0.109	client.fcc.com	client
192.168.0.110	server.fcc.com	server

#Conferir o nome da maquina
hostname
hostname -f

################### NA Maquina KDC ######################################
#Instalar kerberos kdc (key distribution center)

sudo apt install krb5-kdc krb5-admin-server

#Nesse momento � necess�rio colocar o dom�nio local do kerberos. Para o exemplo �:

FCC.COM

#Agora � necess�rio entrar com o dom�nio do servidor de chaver kerberos (kdc). Neste caso:

kdc.fcc.com

#OBS: As configura��es do kerberos podem ser acessadas e alteradas pelo arquivo /etc/krb5.conf. A mais importante para este caso � a linha "default_realm = fcc.com"

#Criar um novo Realm no PC kdc. Ser�o requisitadas senhas administrativas para o BD do kerberos

sudo krb5_newrealm

#Depois de cadastrar uma senha mestra, � nescess�rio cadastrar um admin principal (admin user)
sudo kadmin.local

addprinc adm/admin

#especifique um password para o usuario e saia

quit

#Para setar permiss�es de acesso para o usu�rio rec�m criado � nescess�rio alterar o arquivo:

sudo gedit /etc/krb5kdc/kadm5.acl

#Descomente a ultima linha deste arquivo

*/admin*

#reinicie o servidor kerberos para as configura��es terem efeito

sudo systemctl restart krb5-admin-server.service

kinit adm/admin

#Um comando util para ver mais informa��es sobre o Ticket Granting Service (TGS):

klist

############################## Na maquina cliente #########################

#mudar a lista de dominios de hosts em todas as maquinas
sudo gedit /etc/host

#adicionar os ips das maquinas utilizadas

192.168.0.108	kdc.fcc.com	kdc
192.168.0.109	client.fcc.com	client
192.168.0.110	server.fcc.com	server


#Instalar o kerberos no lado do cliente

sudo apt install krb5-user libpam-krb5 libpam-ccreds 

#Setar o dominio do kerberos

FCC.COM

#Setar o dominio do servidor kdc kerberos

kdc.fcc.com

#setar o servidor administrativo

kdc.fcc.com

#Depois de instalado, a fim de teste, requisitar um ticket ao servidor

kinit adm/admin@FCC.COM

# e visualizar com:

klist