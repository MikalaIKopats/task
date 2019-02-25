<?php

class db_section extends DBObject {

	/**
	 *
	 *  Site ID. Autoset on creation by {@link SiteService::add()}
	 *
     *  @primary
	 *  @var int
	 *  @readonly
	 */

	var $id;

	/**
	 *
	 *  Site description
	 *
	 *  @var string
	 */

	var $description;

	/**
	 *
	 *  ID of site
	 *
	 *  @readonly update
	 *  @var int
	 */

	var $site_id;

	/**
	 *
	 *  Site activity flag.
	 *
	 *  @var boolean
	 *  @default '1'
	 */

	var $active;

    /**
     *
     *  Code
     *
     *  @var string
     *  @nillable
     */

    var $code;

	/**
	 *
	 *  @var string
	 *  @default ''
	 */

	var $url;

    /**
     *  @var int
     *  @default 'NULL'
     *  @nillable
     */

    var $beacon_conversion_id;

    /**
     *  @var int
     */

    var $log_predict_stats;

    /**
     *  @var int
     */

    var $predict_version;
    
    /**
     *  @var int
     */

    var $is_click_fraud;

public function filter_data($data){
       $filtered = parent::filter_data($data);
       $this->validate_presence_of($filtered, 'description');
       $this->validate_presence_of($filtered,'url');
       $this->validate_presence_of($filtered, 'site_id');


       if ($filtered['url']) {
           if (!preg_match('/^http(s?):\/\//', $filtered['url']))
               throw new Exception ("site_url should begins with 'http(s)://'");
           if ($filtered['url'] != strip_tags($filtered['url']))
               throw new Exception ("site_url should not contains html tags");
       }

   	return $filtered;

   }
public function filter_insert_data($data){
	$section = parent::filter_insert_data($data);  // фильтруем данные
	$site = new db_site();
	$site->id=$section['site_id'];   // устанавливаем значение id сайта, пришедшее с данными
	$site->load();                   // проверяем сущесвует ли сайт с данным ID, если нет, то будет ошибка
	return $section;
}
public function filter_update_data($data){
    $this->validate_presence_of($data, 'id');
	$section['id']=$data['id'];
    $this->id = $section['id'];     // устанавливаем id section, пришедшее с данными
    $this->load();					// проверяем существует ли db_section c данным ID,  если нет, то будет ошибка
	$section = parent::filter_update_data($data); // фильтруем данные
	$site = new db_site();
	$site->id=$section['site_id'];	// устанавливаем значение id сайта, пришедшее с данными
	$site->load();					// проверяем сущесвует ли сайт с данным ID, если нет, то будет ошибка
	return $section;
}
};

?>
