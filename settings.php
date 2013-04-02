<?php 

/*
 @nom: settings
 @auteur: Idleman (idleman@idleman.fr)
 @description: Page de gestion de toutes les preferences/configurations administrateur
 */

require_once('header.php'); 

$db = (file_exists(PATH_BDD)?Functions::unstore():array());
$tpl->assign('places', $db['places']);
$tpl->assign('engines', $db['engines']);
$tpl->assign('cron', $db['cron']);

if (isset($_GET['menu']))
{
$tpl->assign('menu',$_GET['menu']);
}
else
{
$tpl->assign('menu',0);
}

if (isset($_GET['error']))
{
$tpl->assign('error',$_GET['error']);
}
else
{
$tpl->assign('error',0);
}

$view = "settings";
require_once('footer.php'); ?>