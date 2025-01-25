<?php
require_once('m304.inc');

Class M304log {
    private $pdo;
    private $m304id;

    public function __construct($pdo=null,$m304id=null) {
        $this->pdo = $pdo;
        $this->m304id = $m304id;
    }

    public function store($facility=7,$tbl=null,$msg0=null,$msg1=null,$msg2=null) {
        $sql = "INSERT INTO logging (M304ID,FACILITY,TBL_NAME,MSG0,MSG1,MSG2) VALUES (:m304id,:facility,:tbl,:msg0,:msg1,:msg2)";
        $sth = $this->pdo->prepare($sql,[PDO::ATTR_CURSOR => PDO::CURSOR_FWDONLY]);
        $sth->bindValue('m304id',$this->m304id,PDO::PARAM_INT);
        if ($facility===null) {
            $sth->bindValue('facility',$facility,PDO::PARAM_NULL);
        } else {
            $sth->bindValue('facility',$facility,PDO::PARAM_INT);
        }
        if ($tbl===null) {
            $sth->bindValue('tbl',$tbl,PDO::PARAM_NULL);
        } else {
            $sth->bindValue('tbl',$tbl,PDO::PARAM_STR);
        }
        if ($msg0===null) {
            $sth->bindValue('msg0',$msg0,PDO::PARAM_NULL);
        } else {
            $sth->bindValue('msg0',$msg0,PDO::PARAM_STR);
        }
        if ($msg1===null) {
            $sth->bindValue('msg1',$msg1,PDO::PARAM_STR);
        } else {
            $sth->bindValue('msg1',$msg1,PDO::PARAM_STR);
        }
        if ($msg2===null) {
            $sth->bindValue('msg2',$msg2,PDO::PARAM_STR);
        } else {
            $sth->bindValue('msg2',$msg2,PDO::PARAM_STR);
        }
        $rv = $sth->execute();

    }

}

?>

