<?php

set_time_limit(0);
ini_set("mongo.native_long", 0);
ini_set("mongo.long_as_object", 1);
header("Content-type: text/html; charset=utf-8");

/*
    数据库临时维护脚本，用在如:
    1.修复玩家数据
    2.发补偿

    注意：由于可能在开服时跑脚本，如果要修复数据的玩家在线，应该先踢下线再操作。
		　配好ip和端口，只要在开始结束间写代码就行
*/

// 开始=================================================================================================================================================

// IP端口
$logic_ip = "127.0.0.1";
$logic_port = 7101;			// 逻辑服内部端口
$mongo_ip = "127.0.0.1";
$mongo_port = 27017;
auto_config_ip_port($logic_ip, $logic_port, $mongo_ip, $mongo_port);	// 自动读端口ip

// 邮件配制
$mail_info[sender] = 'NOV Operation Team';  // 邮件发送人
$mail_info[title] = "Happy Women's Day!"; 	// 邮件标题
$mail_info[contents] = "Darling, no one looks as good as you! Happy Women's Day!"; // 邮件内容
$mail_info[gold] = 0; 						// 赠送钻石数
$mail_info[items] = array('20513401,5', '20701202,2', '20703301,2', '20506401,1');

$socket = new Socket($logic_ip, $logic_port);
//print_r($socket);
if(!$socket) { echo "连不上逻辑服,服务器可能关闭中! <br>\r\n"; }
$Mongo = new MongoClient("mongodb://" . $mongo_ip . ':' . $mongo_port);
if(!$Mongo) { exit("mongo connect failed!\r\n"); }

$collection = $Mongo->mmo->single_dungeon;
if(!$collection) {exit("collection is not exits!\r\n");}
$cursor = $collection->find();
if(!$cursor) {exit("no player selected! you need not to execute this script!\r\n");}
foreach ($cursor as $doc) {
    $role_id = $doc['role_id']->value;

    // 对玩家数据进行修复
	foreach ($doc[nor_elite_item] as $key=>$value) {
		$dungeon_id = ($value[dungeon_id]->value);
		if ($value[dungeon_id]->value == 80202009 && $value[progress] == 0) {
			$doc[nor_elite_item][$key][progress] = 1;
			echo $role_id . "<br>\r\n";
			
			// player_ban_offline($role_id, $Mongo, $socket);																	// 踢下线
			// $collection->update(array("role_id" => new MongoInt64($role_id)), $doc);											// 替换
			// $collection->update(array("role_id" => new MongoInt64($role_id)), array('$set' => array("level" => (int)8)));	// 修改
			// send_mail($role_id, $Mongo, $socket, $mail_info);																// 发邮件
			
			break;
		}
	}
}

// 结束=================================================================================================================================================












// 自动判断ip和端口
function auto_config_ip_port(&$logic_ip, &$logic_port, &$mongo_ip, &$mongo_port) {

	$path_ =  (explode("/",dirname(__FILE__)));
	$path_ = '/'.$path_[1].'/'.$path_[2].'/';
	
	$file_path = $path_.'server_global_conf/server_maintainer.json';
	$server_maintainer_json = preg_replace("/\/\*[\s\S]+?\*\//", "", file_get_contents($file_path));
	$server_maintainer_json = preg_replace("/\/\/(.*)?/", "", $server_maintainer_json);

	$server_maintainer = json_decode($server_maintainer_json, true);
	if ($server_maintainer == "") return 0;
	
	$mongo_ip = $server_maintainer[mongodb_server][ip];
	$mongo_port = $server_maintainer[mongodb_server][port];
	
	$game_solution_id = $server_maintainer[game_solution_id];
	$game_port_prefix = $server_maintainer[game_port_prefix];
	
	
	$file_path = $path_.'server_global_conf/server_identity.json';
	$server_identity_json = preg_replace("/\/\*[\s\S]+?\*\//", "", file_get_contents($file_path));
	$server_identity_json = preg_replace("/\/\/(.*)?/", "", $server_identity_json);
	$server_identity = json_decode($server_identity_json, true);
	if ($server_identity == "") return 0;
	$server_identity = $server_identity[server_identity];
	$mongo_ip = $server_maintainer[game_server_list][$server_identity][telecom_ip];
	
	$path = $path_.'server/config/server/open_solution';  
    if(!is_dir($path)) return;  
    $handle  = opendir($path);  
    while( false !== ($file = readdir($handle))) {  
        if($file != '.'  &&  $file!='..') {
			$file_json = preg_replace("/\/\*[\s\S]+?\*\//", "", file_get_contents($path.'/'.$file));
			$file_json = preg_replace("/\/\/(.*)?/", "", $file_json);
            $open_solution = json_decode($file_json, true);
			if ($open_solution[solution_id] == $game_solution_id) {
				foreach ($open_solution[monitor_list] as $val1) {
					foreach ($val1[acceptor] as $val2) {
						if ($val2[monitor_type] == 1) {
							$logic_port = $game_port_prefix.$val2[inner_port_suffix];
							break;
						}
					}
				}
				
			}
        }  
    }
	
	
	echo "自己读ip端口, 逻辑服:".$logic_ip.':'.$logic_port.' Mongo'.$mongo_ip.':'.$mongo_port."  <br>\r\n";

}

// 判断玩家是否存在并在线
function player_exists_or_online($role_id, &$Mongo) {
    if(!$Mongo) {
    	exit("mongo quote is null\r\n");
    }
    $role_collection = $Mongo->mmo->role;
    if(!$role_collection) {
    	exit("role collection is null\r\n");
    }
    $role_info = $role_collection->findOne(array('role_id' => new MongoInt64($role_id)));
    if (isset($role_info["role_id"]) && intval($role_info["role_id"]->value) > 0) {
        if ($role_info["online"] == 1) {
            return 2;
        } else {
            return 1;
        }
    } else {
        return 0;
    }
}


// 根据玩家id封禁并踢下线
function player_ban_offline($role_id, &$Mongo, &$socket) {
	$player_exists_or_online = player_exists_or_online($role_id, $Mongo);

    $role_collection = $Mongo->mmo->role;
    if ($player_exists_or_online > 0) {
        if ($player_exists_or_online == 1) {
            return 1;
        }

        // 封禁玩家
        player_ban($role_id, $socket);

        for ($i = 0; $i < 10; ++$i) {
            sleep(1);
            $role_info = $role_collection->findOne(array('role_id' => new MongoInt64($role_id)));
            if ($role_info["online"] == 0) {
                sleep(1);
                echo "封禁role_id : " . $role_id . "成功"."<br>\r\n";
                return 1;
            }
        }
        echo "封禁role_id失败:" . $role_id . "，无法修复该玩家数据!"."<br>\r\n";
        return 0;
    } else {
        echo "封禁role_id不存在:" . $role_id . "<br>\r\n";
        return 0;
    }
}


// 封禁玩家
function player_ban($role_id, &$socket) {
    // 给服务端发送封禁消息
    $type = 1; // 封禁类型为1则角色ID封禁
    send_ban_unban($type, $role_id, $socket);
}


// 解封玩家
function player_unban($role_id, &$socket) {
    $type = 11; // 封禁类型为11则角色ID解封
    send_ban_unban($type, $role_id, $socket);
}


// 给服务端发送封禁或解封消息
function send_ban_unban($type, $role_id, &$socket) {
    if($type == 1) {
        $banTime = time() + 10;
    } else if($type == 11) {
        $banTime = time() - 999 * 24 * 3600;
    }
    $msg_id = '30100001';
    return $socket->send(
        $msg_id,
        array(
            array("k" => "a*", "v" => NULL), // 账号
            array("k" => "int64", "v" => $role_id), //64位，角色id
            array("k" => "V", "v" => $type), //32位，方式
            array("k" => "V", "v" => $banTime), //32位，解封时间
        ),
        $role_id
    );
}


// 如果玩家在线, 就根据玩家id发邮件, 如果玩家不在线, 就把邮件写入到mongo
function send_mail($role_id, &$Mongo, &$socket, $mail_info) {
	$player_exists_or_online = player_exists_or_online($role_id, $Mongo);

    // 邮件消息号
    $msg_id = '30100004';

    // 参数
    $type = 0; // 发送类型为 : 0, 即向特定玩家发送邮件
    $start_level = 0;
    $end_level = 0;
    $sender = $mail_info[sender]; // 邮件发送人
    $title = $mail_info[title]; // 邮件标题
    $contents = $mail_info[contents]; // 邮件内容
    $gold = $mail_info[gold]; // 赠送钻石数
    // 赠送道具array("item_id, item_num", "道具ID, 道具数量", ...)
    $items = $mail_info[items];

    if(!$player_exists_or_online) {
        echo "role_id : " . $role_id . " 不存在, 无法发送邮件!\r\n";
        return 2;
    }

    if($player_exists_or_online == 1 || ((!$socket) && $player_exists_or_online != 0)) {
        $itemInfos = array();
        foreach($items as $item)
        {
            list($itemId, $itemNum) = explode(',', $item);
            $itemInfo = array();
            $itemInfo['index'] = (int)0;
            $itemInfo['id'] = (int)$itemId;
            $itemInfo['amount'] = (int)$itemNum;
            $itemInfo['bind'] = (int)2;
            $itemInfo['overdue_time'] = new MongoInt64(0);
            $itemInfos[] = $itemInfo;
        }
        $collection = $Mongo->mmo->operating;
        $collection->update(
            array("role_id" => new MongoInt64($role_id)),
            array(
                '$push' => array(
                    "mail_list" => array(
                        "receiver_id" => new MongoInt64($role_id),
                        "sender_id" => new MongoInt64(0),
                        "send_type" => (int)0,
                        "send_name" => (string)$sender,
                        "gold" => (int)$gold,
                        "title" => (string)$title,
                        "content" => (string)$contents,
                        "item" => $itemInfos,
                        "show_send_time" => new MongoInt64(time()),
                    )
                )
            ),
			array("upsert" => true)
        );
        return 1;
    } else {
        $itemInfos = array();
        foreach($items as $item)
        {
            list($itemId, $itemNum) = explode(',', $item);
            $itemInfo = array();
            $itemInfo['id'] = (string)$itemId;
            $itemInfo['amount'] = (string)$itemNum;
            $itemInfo['bind'] = (string)2;
            $itemInfos[] = $itemInfo;
        }
        $result = $socket->send(
            $msg_id,
            array(
                array("k" => "int64_array", "v" => array($role_id)), //数组,有变动的标签ID
                array("k" => 'V', "v" => (string)$type),
				array("k" => 'V', "v" => (string)0),
                array("k" => 'V', "v" => (string)$start_level),
                array("k" => 'V', "v" => (string)$end_level),
                array("k" => 'a*', "v" => (string)$sender),
                array("k" => 'a*', "v" => (string)$title),
                array("k" => 'a*', "v" => (string)$contents),
                array(
                    "k" => "array[struct]",
                    "v" => $itemInfos,
                    "struct" => array(
                        array("k" => 'V', "v" => 'id'),
                        array("k" => 'V', "v" => 'amount'),
                        array("k" => 'C', "v" => 'bind'),
                    )
                ),
                array("k" => 'V', "v" => $gold),
            ),
            $role_id
        );
		echo "在线发邮件role:".$role_id." <br>\r\n";
        sleep(1);
    }

    return $result;
}


class socket
{
    private $ip = "192.168.8.108";
    private $port = 7101;
    private $timeout = 500;
    private $str = "";
    private $socket = null;

    //构造函数自动执行socket连接
    public function __construct($logic_ip, $logic_port)
    {
        $this->ip = $logic_ip;
        $this->port = $logic_port;
        $this->connect();
    }

    //定义连接函数
    public function connect()
    {
        if ($this->socket == null) {
            $this->socket = socket_create(AF_INET, SOCK_STREAM, SOL_TCP);
            socket_set_option($this->socket, SOL_SOCKET, SO_RCVTIMEO, array("sec" => $this->timeout, "usec" => 0));
            if (@socket_connect($this->socket, $this->ip, $this->port)) {
                return $this;
            } else {
                return false;
            }
        }
        return $this;
    }

    // 发送数据包到服务端
    public function send($msgid, $data, $role_id = 0)
    {
        $pack_msgid = pack("V", $msgid); //生成消息号
        $pack_data = NULL;
        foreach ($data as $v) {
            $pack_data .= $this->switch_pack($v);
        }

        $pack_contents = $pack_msgid . $pack_data; //包内容=消息号+包体(含角色ID,64位)
        //正常包内容 = 消息号 + 64位0 +  包内容长度 + 包内容
        $pack = $pack_msgid . $this->pack_int64($role_id) . pack("V", strlen($pack_contents)) . $pack_contents; //求出包长,加上包内容
        //正常包 = 正常包内容长度 + 正常包内容
        $pack = pack("V", strlen($pack)) . $pack;//加头部包长度

        $res = @socket_write($this->socket, $pack, strlen($pack));
        if ($res == strlen($pack)) {
            return true;
        } else {
            return false;
        }
    }

    public function read()
    {
        $buf = "";
        socket_recv($this->socket, $buf, 8192, 0);
        $data = unpack("V3", $buf);
        return $data;
    }

    public function switch_pack($v)
    {
        $pack_data = '';
        switch ($v['k']) {
            case 'a*':
                $str = pack($v['k'], $v['v']);
                $pack_data .= pack("S", strlen($str)) . $str;
                break;
            case 'string':
                // $str = pack('a*', $v['v']);
                //$pack_data .= pack("S", strlen($str)) . $str;
                $pack_data .= $this->pack_string($v['v']);
                break;
            case 'int_array':
                $pack_data .= $this->pack_int_array($v['v']);
                break;
            case 'int64_array':
                $pack_data .= $this->pack_int64_array($v['v']);
                break;
            case 'string_array':
                $pack_data .= $this->pack_string_array($v['v']);
                break;
            case 'int64':
                $pack_data .= $this->pack_int64($v['v']);
                break;
            case 'array[struct]':
                $pack_data .= $this->pack_array_struct($v['v'], $v["struct"]);
                break;
            default:
                $pack_data .= pack($v['k'], $v['v']);    //生成包体
        }
        return $pack_data;
    }

    public function pack_int64($data)
    {
        $l = $data >> 32; //左32位
        $r = $data & 0xFFFFFFFF; //右32位
        return pack('V', $r) . pack('V', $l);
    }

    public function pack_string($v)
    {
        $str = pack('a*', $v);
        return pack("S", strlen($str)) . $str;
    }

    public function pack_int_array($nums)
    {
        $len = count($nums);
        $pack_data = pack("v", $len);
        foreach ($nums as $v) {
            $pack_data .= pack("V", $v);
        }
        return $pack_data;
    }

    public function pack_int64_array($nums)
    {
        $len = count($nums);
        $pack_data = pack("v", $len);
        foreach ($nums as $v) {
            $pack_data .= $this->pack_int64($v);
        }
        return $pack_data;
    }
    public function pack_string_array($nums)
    {
        $len = count($nums);
        $pack_data = pack("v",$len);
        foreach ($nums as $v)
            $pack_data .= $this->pack_string($v);
        return $pack_data;

    }

    // v struct
    public function pack_array_struct($array, $struct){
        $len = count ( $array );
        $pack_data=pack("v",$len);
        foreach ($array as $k => $v) {
            foreach ($struct as $s) {
                if(isset($s['struct'])){
                    $pack_data .= $this->pack_array_struct($v[$s['v']], $s['struct']);
                }else{
                    $pack_data .= $this->switch_pack(array('k' => $s['k'], 'v' => $v[$s["v"]]));
                }

            }
        }
        return $pack_data;
    }

    public function close()
    {
        @socket_close($this->socket);
        $this->socket = NULL;
    }

    public function __destruct()
    {
        $this->close();
    }
}


function send($msg_id, $msg_content, $role_id, $logic_ip, $logic_port) {
    // 创建socket
    $socket = socket_create(AF_INET, SOCK_STREAM, SOL_TCP);
    // 设置socket, 超时限制500秒
    socket_set_option($socket, SOL_SOCKET, SO_RCVTIMEO, array("sec" => 500, "usec" => 0));
    // 连接socket
    @socket_connect($socket, $logic_ip, $logic_port);

    // 生成消息号
    $pack_msg_id = pack("V", $msg_id);
    // 循环拼接包体
    $pack_data = pack_array($msg_content);
    // 包内容=消息号+包体
    $pack_contents = $pack_msg_id . $pack_data;
    //正常包内容 = 消息号 + 64位角色ID +  包内容长度 + 包内容
    $pack = $pack_msg_id . switch_pack(array("k" => "int64", "v" => $role_id)) . pack("V", strlen($pack_contents)) . $pack_contents; //求出包长,加上包内容
    //正常包 = 正常包内容长度 + 正常包内容
    $pack = pack("V", strlen($pack)) . $pack;

    return @socket_write($socket, $pack, strlen($pack));
}


// 兼容需要加包头和不需要加包头的同一类型的数据的数组的打包
function pack_array($msg_content, $has_head = false){
    $pack_data = "";
    if($has_head) {
        $len = count($msg_content);
        $pack_data = pack("v", $msg_content);
    }
    foreach ($msg_content as $v) {
        $pack_data = switch_pack($v, $pack_data);
    }
    return $pack_data;
}


function switch_pack($v, $pack_data = "")
{
    switch ($v['k']) {
        case 'a*':
            $pack_data .= pack_string($v['v']);
            break;
        case 'string':
            $pack_data .= pack_string($v['v']);
            break;
        case 'int64':
            $pack_data .= pack_int64($v['v']);
            break;
        // 需要加包头的同一类型的数据的数组打包
        case "array":
            $pack_data .= pack_array($v, true);
            break;
        // 需要加包头的不同类型的数据的数组递归打包
        case 'array[struct]':
            $pack_data .= pack_array_struct($v['v'], $v["struct"]);
            break;
        default:
            $pack_data .= pack($v['k'], $v['v']);
    }
    return $pack_data;
}


function pack_string($v) {
    $str = pack("a*", $v['v']);
    return pack("S", strlen($str)) . $str;
}


function pack_int64($v) {
    $l = $v >> 32; //左32位
    $r = $v & 0xFFFFFFFF; //右32位
    return pack('V', $r) . pack('V', $l);
}


function pack_array_struct($array, $struct) {
    $len = count($array);
    $pack_data = pack("v", $len);
    foreach ($array as $v) {
        foreach ($struct as $s) {
            if(isset($s['struct'])){
                $pack_data .= pack_array_struct($v[$s['v']], $s['struct']);
            }else{
                $pack_data .= switch_pack(array('k' => $s['k'], 'v' => $v[$s["v"]]));
            }
        }
    }
    return $pack_data;
}


