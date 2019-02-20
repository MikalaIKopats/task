<?php

include('dbobject.inc');

class db_site extends DBObject {

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
	 *  Site activity flag.
	 *
	 *  @var boolean
	 *  @default '1'
	 */

	var $active;

    /**
     *
     *  ID of site default section
     *
     *  @var int
     *  @default '0'
     */

    var $default_section_id;

	/**
	 *
	 *  If TRUE, section code may be used in tag
	 *
	 *  @var boolean
	 *  @default '0'
	 */

	var $uses_section_codes;

	/**
	 *  @var string
	 */

	var $site_url;

	/**
	 *
	 *  Site code.  May be used if {@link publisher::$uses_site_codes} set to TRUE
	 *
	 *  @var string
	 *  @nillable
	 */

	var $code;

    /**
     *  @var int
     *  @default '1'
     *  @readonly
     */

    var $sections_share_channels;

    /**
     *  @var int
     */

    var $vurl_id;

    /**
     *  @var int
     */

    var $allow_site_to_vurl_lookup;

    
    function filter_data($data) {
		$filtered = parent::filter_data($data);

		$this->validate_presence_of($filtered, 'description');
        
        if ($filtered['site_url']) {
            if (!preg_match('/^http(s?):\/\//', $filtered['site_url']))
                throw new Exception ("site_url should begins with 'http(s)://'");
            if ($filtered['site_url'] != strip_tags($filtered['site_url']))
                throw new Exception ("site_url should not contains html tags");
        }

        $filtered['uses_section_codes'] = $filtered['default_section_id'] && $filtered['uses_section_codes'];

        return $filtered;
     }

    public function filter_update_data($data){
        $this->validate_presence_of($data, 'id');
        $section['id']=$data['id'];
        $this->id = $section['id'];
        $this->load();
        $section = parent::filter_update_data($data);
                return $section;
    }
};

?>
