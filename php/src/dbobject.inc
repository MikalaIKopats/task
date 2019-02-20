<?php

include('config.inc');

class DBObject {

	function db() {
		$db = new DBObject();
        $mysql_db = $db->db_rw();
        return $mysql_db;
	}
    
    function db_rw() {
		global $mysql_db;

		if (isset($mysql_db)) {
			return $mysql_db;
		}
        
        $mysql_db = new mysqli(TRF_HOST, TRF_USER, TRF_PASSWORD, TRF_DB_NAME, TRF_PORT);
        
		if (mysqli_connect_errno()) {
			throw new Exception ('Connect to trf db <b>' . TRF_DB_NAME . '</b> failed: ' . mysqli_connect_error());
		}

		if (!$mysql_db->select_db(TRF_DB_NAME)) {
			throw new Exception ('Couldn\'t select ' . TRF_DB_NAME);
		}
        
		return $mysql_db;
	}
    
    function table_name() {
		$name = get_class($this);
		return preg_replace('/^db_/', '', $name);
	}

	function primary_key_fields() {
		$class_name = get_class($this);
		$class = new ReflectionClass($class_name);
		$properties = $class->getProperties();
		$fields = array();

		foreach ($properties as $prop) {
			$doc = $prop->getDocComment();
			if (preg_match('|@primary|', $doc, $match)) {
				$fields[] = $prop->getName();
			}
		}

		return $fields;
	}

	function soap_field_to_db_field($field) {
		return $field;
	}

	function db_field_to_soap_field($field) {
		return $field;
	}

	function populate_from_row($row) {
		$class_name = get_class($this);
		$class = new ReflectionClass($class_name);
		foreach ($row as $col => $val) {
			$col = $this->db_field_to_soap_field($col);
			if ($class->hasProperty($col)) {
                $this->$col = $val;
			}
        }
	}
    
	protected function query_where($fields = array(), $negative_fields = array(), $like_fields = array()) {
		$where = array();
		foreach ($fields as $field) {
            $query_part = $field;
            $is_negative = in_array($field, $negative_fields);
            if ($this->$field === null) {
                $query_part .= ' IS ' . ($is_negative ? 'NOT ' : '') . 'NULL';
            } elseif (is_array($this->$field)) {
                if (!count($this->$field)) {
                    $this->$field = "''";
                }
                $query_part .= ($is_negative ? ' NOT' : '') .
                    " IN ('" . implode("','", array_map('mysql_escape_string', $this->$field)) . "')";
            } else {
                $equalty = ($is_negative ? '!' : '') . "='";
                if (in_array($field, $like_fields)) {
                    $equalty = ($is_negative ? ' NOT ' : ' ') . " LIKE '";
                }
                //$query_part .= $equalty . mysql_escape_string($this->$field) . "'";
                //This function was deprecated in PHP 4.3.0 and removed in PHP 7.0.0. Instead
                $query_part .= $equalty . mysqli_escape_string($this->db(),$this->$field) . "'";
            }
        $where[] = $query_part;
		}
		return implode(' AND ', $where);
	}

	protected function query_where_primary() {
		return $this->query_where($this->primary_key_fields());
	}

	function load() {
		$query = "SELECT * FROM " . $this->table_name() . " WHERE " . $this->query_where_primary();
		$result = $this->db()->query($query);
        
        if ($result->num_rows == 0) {
			throw new Exception("No " . $this->table_name() . " " . $this->query_where_primary() . " found");
		} else if ($result->num_rows > 1) {
			throw new Exception("More then one " . $this->table_name() . " record matching " .
                                $this->query_where_primary() . " found");
        }
        
		$row = $result->fetch_assoc() ;
		$this->populate_from_row($row);

		$result->close();
	}

	function delete_where($condition_fields = array()) {
		$query = 'DELETE FROM ' . $this->table_name();
		$query_where = $this->query_where($condition_fields);
		if ($query_where) {
			$query .= ' WHERE ' . $query_where;
		}
		$this->db()->query($query);
	}
    
	function update($data) {
		if(count($data) < 1) {
			return;
		}
		
		$class_name = get_class($this);
		$class = new ReflectionClass($class_name);

        foreach ($data as $name => $value) {
			if ($class->hasProperty($name)) {
				//$values[] = $this->soap_field_to_db_field($name) . "=". ($value === null ? 'NULL' : "'" . mysql_escape_string($value) . "'");
                $values[] = $this->soap_field_to_db_field($name) . "=". ($value === null ? 'NULL' : "'" . mysqli_escape_string($this->db(),$value) . "'");
			}
        }

		$query = "UPDATE  " . $this->table_name() . " SET " . implode(', ', $values) . " WHERE " . $this->query_where_primary();
		$this->db()->query($query);
        $this->populate_from_row($data);

	}

	function insert($data) {
		$names = array();
		$values = array();

		$class_name = get_class($this);
		$class = new ReflectionClass($class_name);

		foreach ($data as $name => $value) {
			if ($class->hasProperty($name)) {
				$names[] = $this->soap_field_to_db_field($name);
				//$values[] = ($value === null ? 'NULL' : "'" . mysql_escape_string($value) . "'");
                // This function was deprecated in PHP 4.3.0 and removed in PHP 7.0.0. Instead
                $values[] = ($value === null ? 'NULL' : "'" . mysqli_escape_string($this->db(),$value) . "'");
			}
		}

		$query = "INSERT INTO " . $this->table_name() . " (" . implode(',', $names) . ")" .
                 " VALUES (" . implode(',', $values) . ")" ;
		$this->db()->query($query);
		$this->id = $this->db()->insert_id;
		return $this->id;
	}

	function select_where($condition_fields = array(), $return_field = '', $negative_fields = array(), $like_fields = array()) {
        $query = $this->query_where($condition_fields, $negative_fields, $like_fields);
		if ($query) {
			$query = ' WHERE ' . $query;
		}
       
		$query = 'SELECT * FROM ' . $this->table_name() . $query . ' ORDER BY ' . implode(', ', $this->primary_key_fields());
		$result = $this->db()->query($query);
		$values = array();
		while ($row = $result->fetch_assoc()) {
			$values[] = $return_field ? $row[$return_field] : $row;
		}
		return $values;
	}

	function unique_tuple() {
		return array();
	}

	function validate_presence_of($data, $field) {
		if ((!isset($this->$field) && !isset($data[$field])) ||
			(isset($data[$field]) && trim($data[$field]) == '')) {
			throw new Exception("Field '$field' cannot be empty");
		}
	}
 
    function readonly_fields($operation = '') {
        $class_name = get_class($this);
        $class = new ReflectionClass($class_name);
        $properties = $class->getProperties();
        $fields = array();
        if ($operation != '') {
            $regexp = "/@readonly\s+$operation/";
        } else {
            $regexp = '/@readonly\s*\r?\n/';
        }

        foreach ($properties as $prop) {
            $doc = $prop->getDocComment();

            if (preg_match($regexp, $doc, $match)) {
                $fields[] = $prop->getName(); 
            }
        }
        return $fields;
    }

    function filter_data($data) {
        $filtered = array();
        if (!is_array($data)) {
            $data = get_object_vars($data);
        }
        // skip readonly and undefined fields
        $readonly = $this->readonly_fields();
        $class_name = get_class($this);
        $class = new ReflectionClass($class_name);
        $properties = $class->getProperties();

        foreach($properties as $property) {
            $prop_name = $property->getName();
            if (!in_array($prop_name, $readonly) && array_key_exists($prop_name, $data)) {
                $filtered[$prop_name] = $data[$prop_name];
            }
        }
        return $filtered;
    }

    function filter_insert_data($data) {
        if (!is_array($data)) {
            $data = get_object_vars($data);
        }
        // set default values on missed not readonly fields
        $class_name = get_class($this);
        $class = new ReflectionClass($class_name);
        $properties = $class->getProperties();
        $readonly = $this->readonly_fields();

        foreach($properties as $property) {
            $prop_name = $property->getName();
            $doc = $property->getDocComment();

            if (!in_array($prop_name, $readonly)) {
                if (preg_match('|@default\s(.*)|', $doc, $match)) {
                    if(isset($data[$prop_name])){       // Проверяес существует ли переменая если да то присваем её свойству
                        $this->$prop_name =$data[$prop_name];
                    }
                    else {
                        @eval('$value = ' . $match[1] . ';');
                        $data[$prop_name] = $value;
                        $this->$prop_name = $value;
                    }
                }
            }
            if (array_key_exists($prop_name, $data) && is_null($data[$prop_name])
                && !preg_match('|@nillable(.*)|', $doc)) {
                unset($data[$prop_name]);
            }
        }

        $filtered = $this->filter_data($data);
                
        return $filtered;
    }

    function filter_update_data($data) {
        if (!is_array($data)) {
            $data = get_object_vars($data);
        }

        // skip readonly update fields
        $filtered = array();
        $readonly = $this->readonly_fields('update');

        foreach($data as $key => $value) {
                  $filtered[$key] = $value;
        }

        // set values for object's undefined fields from db
        $class_name = get_class($this);
        $class = new ReflectionClass($class_name);
        $properties = $class->getProperties();
        foreach($properties as $property) {
            $prop_name = $property->getName();
            $doc = $property->getDocComment();
            if (array_key_exists($prop_name, $data) && is_null($data[$prop_name])
                && !preg_match('(.*)@nillable(.*)', $doc)) {
                $data[$prop_name] = $this->$prop_name;
            }
            if (!array_key_exists($prop_name, $filtered)) {
                $filtered[$prop_name] = $this->$prop_name;
            }
        }


        $filtered = $this->filter_data($filtered);

        return $filtered;
    }


    function check_double_data($data){
	    foreach ($data as $key =>$value){
            $query = "SELECT * FROM " . $this->table_name() . " WHERE " .$key ."='".$value."'  ";
            $result = $this->db()->query($query);
            if (!$result->num_rows == 0){
                throw new Exception("Double is found in the table ".$this->table_name()." record");
            }

        }
        return true;

    }

    function check_type_data($data){

        if (!is_array($data)) {
            $data = get_object_vars($data);
        }

        $class_name = get_class($this);
        $class = new ReflectionClass($class_name);
        $properties = $class->getProperties();
        $readonly = $this->readonly_fields();


        foreach($properties as $property) {
            $prop_name = $property->getName();
            $doc = $property->getDocComment();


            if(!in_array($prop_name,$readonly)){
                if(preg_match('|@var(.*)|',$doc,$match)){

                    if(isset($data[$prop_name]))
                        switch (trim($match[1])){
                            case 'int':
                                if(is_int($data[$prop_name])){
                                    $this->$prop_name =$data[$prop_name];
                                }
                                else{
                                    $this->$prop_name =null;
                                    $data[$prop_name]=null;
                                }
                                break;
                            case 'string':
                                if(is_string($data[$prop_name])){
                                    $this->$prop_name =$data[$prop_name];
                                }
                                else{
                                    $this->$prop_name =null;
                                    $data[$prop_name]=null;
                                }
                                break;
                            case 'boolean':
                                if(is_bool($data[$prop_name])){
                                    $this->$prop_name =$data[$prop_name];
                                }
                                else{
                                    $this->$prop_name =null;
                                    $data[$prop_name]=null;
                                }
                                break;
                            default:
                                 $this->$prop_name =null;
                                 $data[$prop_name]=null;

                        }

                }

            }



        }
        return $data;

    }

}

?>
