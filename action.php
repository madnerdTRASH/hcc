<?php
require_once('header.php');


switch($_['action']){

//Gestion du login
case 'LOGIN':
	if($_['email']==EMAIL && $_['password']==PASSWORD){
		$_SESSION['myUser'] = serialize(array('login'=>EMAIL));
	}else{
		exit('Bad code');
	}
	header('location: index.php');
break;

//Gestion de la deconnexion
case 'DISCONNECT':
	$_SESSION = array();
	session_unset();
	session_destroy();
	header('location: index.php');
break;

/*

Gestion des prises

*/

//
//Ajouter une prise
//
case 'ADD_ENGINE':
	$db = (file_exists(PATH_BDD)?Functions::unstore():array());

	//Copie de l'image sur le serveur
    $fichier = basename($_FILES['picture']['name']);
    move_uploaded_file($_FILES['picture']['tmp_name'], PICTURE_FOLDER .'/'. $fichier);

	//Copie des donnees
	$newEngine['picture'] = PICTURE_FOLDER .'/'.$fichier;
	$newEngine['name'] = $_['name'];
	$newEngine['code'] = $_['code'];
	$newEngine['state'] = 'off';
	$newEngine['description'] = $_['description'];
	$newEngine['place'] = $_['idPlace'];
	$newEngine['typeEngine'] = $_['typeEngine'];
	
	//Si Aucune cle n'existe alors cree la cle 1 sinon creer la cle suivante
	$db['keys']['engines']=(isset($db['keys']['engines'])?$db['keys']['engines']+1:1);
	$db['engines']['id-'.$db['keys']['engines']] =  $newEngine;
	
	//Enregistrement dans la base JSON
	Functions::store($db);
	header('location: settings.php');
break;

//
//Modifier une prise
//
case 'MODIFY_ENGINE':
	$db = (file_exists(PATH_BDD)?Functions::unstore():array());

//Verifie si la commande n'a pas ete lance a la main
if (!empty($_POST))
{	
	//Si une image est defini alors la sauvegarder
	if ($_FILES['picture']['error'] == 0)
	{
	$fichier = basename($_FILES['picture']['name']);
	move_uploaded_file($_FILES['picture']['tmp_name'], PICTURE_FOLDER .'/'. $fichier);
	$db['engines'][$_['idengine']]['picture'] = PICTURE_FOLDER .'/'.$fichier;
	}
	
	$db['engines'][$_['idengine']]['name'] = $_['name'];
	$db['engines'][$_['idengine']]['code'] = $_['code'];
	$db['engines'][$_['idengine']]['description'] = $_['description'];
	$db['engines'][$_['idengine']]['typeEngine'] = $_['typeEngine'];
	
	//Si une piece est defini alors la sauvegarder
	if (!empty($_['idPlace']))
	{
	$db['engines'][$_['idengine']]['place'] = $_['idPlace'];
	}

	//Enregistrement dans la base JSON
	Functions::store($db);
	
	//Retour a la configuration de la prise
	header('Location: ' . $_SERVER['HTTP_REFERER']);
	
}
else
{
header('location: index.php');
}
break;

case 'SHOW_ENGINE':
	$db = (file_exists(PATH_BDD)?Functions::unstore():array());
break;

//
//Modifier l'etat d'une prise
//
case 'CHANGE_STATE':

//Ouvrir le fichier JSON
$db = (file_exists(PATH_BDD)?Functions::unstore():array());

//Vérifie qu'une ID et un état a été précisé
if (isset($_['engine']) && isset($_['state']))
{

	//Récupération des informations de la prise à partir de l'ID
	foreach($db['engines'] as $id=>$engine){
		if ($_['engine'] == $id)
		{
			$code = $engine['code'];
			$typeengine = $engine['typeEngine'];
		}
	}

			//Si c'est une prise chacon
			if( $typeengine == 0)
			{
					system(' ./chacon '.PIN.' '.SENDER.' '.$code.' '.$_['state']); //Activation
			}

	//Si prise SCS
			if($typeengine == 1){
	//Execution de la commande
				system(' ./scs '.$code.' '.$_['state']);
			}

		//Changement de l'état dans la base de données
				$db['engines'][$_['engine']]['state'] = $_['state'];

	//Sauvegarde dans la BD JSON
			Functions::store($db);

	//Renvoi sur la page (ou message A vos ordres si YURI est utilisée)
			if(!isset($_['provider'])){
				header('location: index.php?place='.$db['engines'][$_['engine']]['place']);
			}else{
				echo 'A vos ordres';
			}
			
		}


		break;

	//
	//Effacer une prise
	//
		case 'DELETE_ENGINE':
		$db = (file_exists(PATH_BDD)?Functions::unstore():array());
		unset($db['engines'][$_['engine']]);
		Functions::store($db);
		header('location: settings.php');
		break;

//
//Effacer une prise
//
case 'DELETE_ENGINE':
	$db = (file_exists(PATH_BDD)?Functions::unstore():array());
	unset($db['engines'][$_['engine']]);
	Functions::store($db);
	header('location: settings.php');
break;

/*

Gestion des piece

*/

//
//Ajouter une piece
//
case 'ADD_PLACE':
	$db = (file_exists(PATH_BDD)?Functions::unstore():array());
	$newPlace['name'] = $_['place'];
	$db['keys']['places']=(isset($db['keys']['places'])?$db['keys']['places']+1:1);
	$db['places']['id-'.$db['keys']['places']] =  $newPlace;
	Functions::store($db);
	header('location: settings.php?menu=1');
break;

//
//Effacer une piece
//
case 'DELETE_PLACE':
	$db = (file_exists(PATH_BDD)?Functions::unstore():array());
	unset($db['places'][$_['place']]);
	Functions::store($db);
	header('location: settings.php?menu=1');
break;


case 'MODIFY_PLACE':
	$db = (file_exists(PATH_BDD)?Functions::unstore():array());

//Verifie si la commande n'a pas ete lance a la main
if (!empty($_POST))
{	
	
	
	$db['places'][$_['idplace']]['name'] = $_['place'];
	//Enregistrement dans la base JSON
	Functions::store($db);
	
	//Retour a la configuration de la prise
	header('Location: ' . $_SERVER['HTTP_REFERER']);
	
}
else
{
header('location: index.php');
}
break;

/*

Creation d'une programmation

*/

//
//Ajout d'une programmation
//
case 'ADD_CRON':
	
//Verifie si la commande n'a pas ete lance a la main ou si aucune prise n'a ete selectionnee
if (!empty($_POST))
{

if ($_['idCronedEngine'] != "noid")
{

$db = (file_exists(PATH_BDD)?Functions::unstore():array());

//Engistrement de la programmation dans la base JSON
$newCron['state'] = $_['state'];
$newCron['idCronedEngine'] = $_['idCronedEngine'];
$newCron['time'] = $_['time'];
$newCron['date'] = $_['date'];

$db['keys']['cron']=(isset($db['keys']['cron'])?$db['keys']['cron']+1:1);
$db['cron']['id-'.$db['keys']['cron']] =  $newCron;

Functions::store($db);

//Extraction de l'heure et des minutes
$hour = substr($newCron['time'],0,2);
$minutes = substr($newCron['time'],3,4);

//Extraction de l'etat
if ($newCron['state'] == 0){$state="off";}
if ($newCron['state'] == 1){$state="on";}

//Extraction de la date
$date = $newCron['date'] ;

//Extraction du dossier courant
$dir = getcwd();

//Extraction du type de prise
$typeEngine = $db['engines'][$newCron['idCronedEngine']]['typeEngine'];
if ($typeEngine == 0){$program=$dir ."/chacon ".PIN." ".SENDER;}
if ($typeEngine == 1){$program=$dir ."/scs";}

//Extraction du code de la prise
$code = $db['engines'][$newCron['idCronedEngine']]['code'];

//Ajout de la programmation dans la table CRON
system('./cron_add '.$minutes.' '.$hour.' '.$date.' "'.$program.' '.$code.' '.$state.'"');

header('location: settings.php?menu=2');
}
else
{
header('location: settings.php?menu=2&error=1');
}
}
else
{
header('location: index.php');
}
break;

//
//Effacer une programmation
//
case 'DELETE_CRON':
$db = (file_exists(PATH_BDD)?Functions::unstore():array());

//Recuperation de la programmation dans la base JSON
$newCron['state'] = $db['cron'][$_['cron']]['state'];
$newCron['idCronedEngine'] = $db['cron'][$_['cron']]['idCronedEngine'];
$newCron['time'] = $db['cron'][$_['cron']]['time'];
$newCron['date'] = $db['cron'][$_['cron']]['date'];

//Extraction de l'heure et des minutes
$hour = substr($newCron['time'],0,2);
$minutes = substr($newCron['time'],3,4);

//Extraction de l'etat
if ($newCron['state'] == 0){$state="off";}
if ($newCron['state'] == 1){$state="on";}

//Extraction de la date
$date = $newCron['date'] ;

//Extraction du dossier courant
$dir = getcwd();

//Extraction du type de prise
$typeEngine = $db['engines'][$newCron['idCronedEngine']]['typeEngine'];
if ($typeEngine == 0){$program=$dir ."/chacon ".PIN." ".SENDER;}
if ($typeEngine == 1){$program=$dir ."/scs";}

//Extraction du code de la prise
$code = $db['engines'][$newCron['idCronedEngine']]['code'];

//Supression de la programmation dans la table CRON
system('./cron_del '.$minutes.' '.$hour.' '.$date.' "'.$program.'" '.$code.' '.$state);	

//Supression de la programmation dans la base JSON
unset($db['cron'][$_['cron']]);
Functions::store($db);
	
header('location: settings.php?menu=2');
break;

/*

Gestion du controle vocale

*/


	case 'GET_YURI_XML':
	$db = (file_exists(PATH_BDD)?Functions::unstore():array());
	$hccPath = substr($_SERVER['HTTP_REFERER'],0,strrpos($_SERVER['HTTP_REFERER'], '/')).'/action.php';

	$template = '<grammar version="1.0" xml:lang="fr-FR" mode="voice" root="ruleEedomus" xmlns="http://www.w3.org/2001/06/grammar" tag-format="semantics/1.0">
	<rule id="ruleEedomus" scope="public">
		<example>Yuri allume le salon</example>
		<tag>out.action=new Object(); </tag>
		<item>Yuri</item>
		<one-of>
		<item>allume<tag>out.action.state="on"</tag></item>
		<item>eteint<tag>out.action.state="off"</tag></item>
	</one-of>

	<one-of>';

	foreach($db['engines'] as $id=>$engine){
		$template .= '<item>'.$engine['name'].'
		<tag>out.action.engine=\''.$id.'\'</tag>
	</item>';
}



$template .= '

</one-of>
<tag>out.action.action=\'CHANGE_STATE\'</tag>
<tag>out.action.provider=\'yuri\'</tag>
<tag>out.action._attributes.threashold="0.80";</tag>
<tag>out.action._attributes.uri="'.$hccPath.'";</tag>
</rule>
</grammar>';

header('Content-Description: File Transfer');
header('Content-Type: application/octet-stream');
header('Content-Disposition: attachment; filename=hcc_yuri_xml.xml');
header('Content-Transfer-Encoding: binary');
header('Expires: 0');
header('Cache-Control: must-revalidate');
header('Pragma: public');
header('Content-Length: ' . strlen($template));
ob_clean();
flush();
echo $template;

break;
default:
echo 'Aucune action correcte n\'est spécifiée';
break;

}








?>
