<?php 

/*
 @nom: modify
 @auteur: maditnerd
 @description: Page pour modifier des informations
 */
 
 require_once('header.php'); 
 
$db = (file_exists(PATH_BDD)?Functions::unstore():array());
$tpl->assign('places', $db['places']);
$tpl->assign('engines', $db['engines']);

if (isset($_GET['engine']) || isset($_GET['place']) || isset($_GET['cron']) )
{
//Choisis la cle de la prise
if (isset($_GET['engine']))
	{

	//Recuperation de la cle de la prise
	$key = $_GET['engine'];

	//Recuperation du nom de la piece
	$placekey = $db['engines'][$key]['place'];

	//Recuperation des valeurs
	$tpl->assign('idengine',$key);
	$tpl->assign('picture',$db['engines'][$key]['picture']);
	$tpl->assign('name',$db['engines'][$key]['name']);
	$tpl->assign('code',$db['engines'][$key]['code']);
	$tpl->assign('typeEngine',$db['engines'][$key]['typeEngine']);
	$tpl->assign('description',$db['engines'][$key]['description']);
	$tpl->assign('piece',$db['places'][$placekey]['name']);
	}

 
//Choisis la cle de la piece
if (isset($_GET['place']))
	{

	//Recuperation de la cle de la piece
	$key = $_GET['place'];

	//Recuperation des valeurs
	$tpl->assign('idplace',$key);
	$tpl->assign('name',$db['places'][$key]['name']);
	}
}
else
{
//Si aucune clé n'est precise revenir sur l'index
header('location: index.php');
}






 
 
 $view = "modify";
 require_once('footer.php'); ?>