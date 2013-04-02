<?php
/*
 @nom: index
 @auteur: Idleman (idleman@idleman.fr)
 @description:  Page d'accueil et de lecture des flux
*/

require_once('header.php'); 
$db = (file_exists(PATH_BDD)?Functions::unstore():array());

asort($db['engines']);
$tpl->assign('engines',$db['engines']);
$places = (isset($db['places'])?$db['places']:array());
asort($places);
$tpl->assign('places',$places);

if (isset($_GET['place']))
{
$tpl->assign('place',$_GET['place']);
}
else
{
$tpl->assign('place',key($places));
}

$view = 'index';
require_once('footer.php'); 


?>