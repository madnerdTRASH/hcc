<?php
require_once('constant.php');
/*
 @nom: constant
 @auteur: Idleman (idleman@idleman.fr)
 @description:  Classe de stockage des fonctions utiles (toutes disponibles en static)
 */

class Functions
{
	private $id;
	public $debug=0;
	const CRYPTKEY = 'zr_e65$^vg41^948e*586"';
	/**
	 * Securise la variable utilisateur entr�e en parametre
	 * @author Valentin
	 * @param<String> variable a s�curiser
	 * @param<Integer> niveau de securisation
	 * @return<String> variable securis�e
	 */

	public static function secure($var,$level = 1){
		$var = htmlentities($var, ENT_QUOTES, "UTF-8");
		if($level<1)$var = mysql_escape_string($var);
		if($level<2)$var = addslashes($var);
		return $var;
	}


	/**
	 * Convertis la chaine pass�e en timestamp quel que soit sont format
	 * (prend en charge les formats type dd-mm-yyy , dd/mm/yyy, yyyy/mm/ddd...)
	 */
	public static function toTime($string){
		$string = str_replace('/','-',$string);
		$string = str_replace('\\','-',$string);

		$string = str_replace('Janvier','Jan',$string);
		$string = str_replace('Fevrier','Feb',$string);
		$string = str_replace('Mars','Mar',$string);
		$string = str_replace('Avril','Apr',$string);
		$string = str_replace('Mai','May',$string);
		$string = str_replace('Juin','Jun',$string);
		$string = str_replace('Juillet','Jul',$string);
		$string = str_replace('Aout','Aug',$string);
		$string = str_replace('Septembre','Sept',$string);
		$string = str_replace('Octobre','Oct',$string);
		$string = str_replace('Novembre','Nov',$string);
		$string = str_replace('Decembre','Dec',$string);
		return strtotime($string);
	}

	/**
	 * Recupere l'ip de l'internaute courant
	 * @author Valentin
	 * @return<String> ip de l'utilisateur
	 */

	public static function getIP(){
		if(isset($_SERVER['HTTP_X_FORWARDED_FOR'])){
			$ip = $_SERVER['HTTP_X_FORWARDED_FOR'];}
			elseif(isset($_SERVER['HTTP_CLIENT_IP'])){
				$ip = $_SERVER['HTTP_CLIENT_IP'];}
				else{ $ip = $_SERVER['REMOTE_ADDR'];}
				return $ip;
	}

	/**
	 * Retourne une version tronqu�e au bout de $limit caracteres de la chaine fournie
	 * @author Valentin
	 * @param<String> message a tronquer
	 * @param<Integer> limite de caracteres
	 * @return<String> chaine tronqu�e
	 */
	public static function truncate($msg,$limit){
		$msg = utf8_encode(html_entity_decode($msg));
		if(strlen($msg)>$limit){
			$nb=$limit-3 ;
			$fin='...' ;
		}else{
			$nb=strlen($msg);
			$fin='';
		}
		return substr($msg, 0, $nb).$fin;
	}


	function getExtension($fileName){
		$dot = explode('.',$fileName);
		return $dot[sizeof($dot)-1];
	}

	/**
	 * Definis si la chaine fournie est existante dans la reference fournie ou non
	 * @param unknown_type $string
	 * @param unknown_type $reference
	 * @return false si aucune occurence du string, true dans le cas contraire
	 */
	public static function contain($string,$reference){
		$return = true;
		$pos = strpos($reference,$string);
		if ($pos === false) {
			$return = false;
		}
		return strtolower($return);
	}

	/**
	 * D�finis si la chaine pass�e en parametre est une url ou non
	 */
	public static function isUrl($url){
		$return =false;
		if (preg_match('/^(http|https|ftp)://([A-Z0-9][A-Z0-9_-]*(?:.[A-Z0-9][A-Z0-9_-]*)+):?(d+)?/?/i', $url)) {
			$return =true;
		}
		return $return;
	}

	/**
	 * D�finis si la chaine pass�e en parametre est une couleur h�xad�cimale ou non
	 */
	public static function isColor($color){
		$return =false;
		if (preg_match('/^#(?:(?:[a-fd]{3}){1,2})$/i', $color)) {
			$return =true;
		}
		return $return;
	}

	/**
	 * D�finis si la chaine pass�e en parametre est un mail ou non
	 */
	public static function isMail($mail){
		$return =false;
		if (filter_var($mail, FILTER_VALIDATE_EMAIL)) {
			$return =true;
		}
		return $return;
	}

	/**
	 * D�finis si la chaine pass�e en parametre est une IP ou non
	 */
	public static function isIp($ip){
		$return =false;
		if (preg_match('^(?:25[0-5]|2[0-4]\d|1\d\d|[1-9]\d|\d)(?:[.](?:25[0-5]|2[0-4]\d|1\d\d|[1-9]\d|\d)){3}$',$ip)) {
			$return =true;
		}
		return $return;
	}

	public static function sourceName($string){
		$name = strtolower($string);
		$name = str_replace(' ','-',$name);
		$name = str_replace('&#039;','-',$name);
		$name = str_replace('\'','-',$name);
		$name = str_replace(',','-',$name);
		$name = str_replace(':','-',$name);
		$name = str_replace('&agrave;','a',$name);
		$name = trim($name);
		$name = html_entity_decode($name,null,'UTF-8');
		return $name;
	}

	public static function makeCookie($name, $value, $expire='') {
		if($expire == '') {
			setcookie($name, $value, mktime(0,0,0, date("d"),
			date("m"), (date("Y")+1)),'/');
		}else {
			setcookie($name, '', mktime(0,0,0, date("d"),
			date("m"), (date("Y")-1)),'/');
		}
	}

	public static function destroyCookie($name){
		Fonction::makeCookie($name,'',time()-3600);
		unset($_COOKIE[$name]);
	}

	static function wordwrap($str, $width = 75, $break = "\n", $cut = false)
	{
		$str = html_entity_decode($str);
		$str =  htmlentities (wordwrap($str,$width,$break,$cut));
		$str = str_replace('&lt;br/&gt;','<br/>',$str);
		$str = str_replace('&amp;','&',$str);
		return $str;
	}

	public static function createFile($filePath,$content){
		$fichier = fopen($filePath,"w+");
		$fwriteResult = fwrite($fichier,$content);
		fclose($fichier);
	}

	public static function crypt($string,$key=Functions::CRYPTKEY){
		$key = sha1($key);
		$return = '';
		for ($i = 0; $i<strlen($string); $i++) {
			$kc = substr($key, ($i%strlen($key)) - 1, 1);
			$return .= chr(ord($string{$i})+ord($kc));
		}
		return base64_encode($return);
	}

	public static function decrypt($string,$key=Functions::CRYPTKEY){
		$key = sha1($key);
		$return = '';
		$string = base64_decode($string);
		for ($i = 0; $i<strlen($string); $i++) {
			$kc = substr($key, ($i%strlen($key)) - 1, 1);
			$return .= chr(ord($string{$i})-ord($kc));
		}
		return $return;
	}


	public static function hexaValue($str){
		$code = dechex(crc32($str));
		  $code = substr($code, 0, 6);
		  return $code;
	}
	
	public static function scanRecursiveDir($dir){
		$files = scandir($dir);
		$allFiles = array();
		foreach($files as $file){
			if($file!='.' && $file!='..'){
				if(is_dir($dir.$file)){
					$allFiles = array_merge($allFiles,Fonction::scanRecursiveDir($dir.$file));
				}else{
					$allFiles[]=str_replace('//','/',$dir.'/'.$file);
				}
			}
		}
		return $allFiles;
	}

		public static function stripAccents($string){
			return strtr(html_entity_decode($string),'���������������������������������������������������',
		'aaaaaceeeeiiiinooooouuuuyyAAAAACEEEEIIIINOOOOOUUUUY');
		}



      public static function store($datas){
      	if(!file_exists(PATH_BDD)) touch(PATH_BDD);
      	file_put_contents(PATH_BDD,json_encode($datas));
      }
      public static function unstore(){
      	if(!file_exists(PATH_BDD)) touch(PATH_BDD);
      	return json_decode(file_get_contents(PATH_BDD),true);
      }




}
?>