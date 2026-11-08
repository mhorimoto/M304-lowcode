<!DOCTYPE html>
<html lang="ja">
  <head>
    <meta charset="utf-8">
    <title>M304-{$data_a.M304ID} console</title>
    <meta name="description" content="https://at.strix-inc.jp/8487/">
    <meta name="keywords" content="M304">
    <meta name="viewport" content="width=device-width,initial-scale=1.0">
    <meta name="format-detection" content="telephone=no,email=no,address=no">
        
    <!-- Import Google Fonts -->
    <link rel="preconnect" href="https://fonts.googleapis.com">
    <link rel="preconnect" href="https://fonts.gstatic.com" crossorigin>
    <link href="https://fonts.googleapis.com/css2?family=Noto+Sans+JP:wght@400;500;700&display=swap" rel="stylesheet">
    
    <!-- Import CSS -->
    <link rel="stylesheet" href="css/style.css">
    
    <!-- Import JS/jQuery Library -->
    <script src="https://code.jquery.com/jquery-1.12.4.min.js"></script>
  </head>
  <body>
    <table class="table-body">
      <tbody>
        {if $data_a.DHCPFLAG==0}
        {$dhcpf = "固定IP"}
        {else}
        {$dhcpf = "DHCP"}
        {/if}
        <input type="hidden" name="MACADDR" value="{$data_a.MACADDR}">
        <tr>
          <th rowspan="4">
	    機台番号<br>
	    <span style="font-size: 32px;">{$data_a.M304ID}</span><br>
	    <a href="m304body2.php?ID={$data_a.M304ID}" target="_self">
	      <input type="button" value="設定変更">
	    </a>
	  </th>
          <th colspan="2">{$data_a.NODE_NAME}</th><th colspan="3">{$data_custom.NAME}</th>
        </tr>
        <tr>
          <th>{$dhcpf}</th><th>IP:{$data_a.FIXED_IPADDRESS}</th>
          <th>NETMASK:{$data_a.FIXED_NETMASK}</th><th>GW:{$data_a.FIXED_DEFGW}</th><th>DNS:{$data_a.FIXED_DNS}</th>
        </tr>
        <tr>
          <td colspan="5">MACADDR:{$data_a.MACADDR}</td>
        </tr>
        <tr>
          <th colspan="5">{$data_a.COMMENT|default:'<br>'}</th>
        </tr>
      </tbody>
    </table>
    <table class="table-body">
      <tr>
        <td>動作状況</td>
        <td colspan="8"><div id="RUNCND"></div></td>
      </tr>
      <tr>
        <td rowspan="2">リレー状況</td>
	{for $i=1 to 8}
        <td>RLY{$i}<br>{$RLYname[$i]}</td>
	{/for}
      </tr>
      <tr>
        <td><div id="rly1"></div></td>
        <td><div id="rly2"></div></td>
        <td><div id="rly3"></div></td>
        <td><div id="rly4"></div></td>
        <td><div id="rly5"></div></td>
        <td><div id="rly6"></div></td>
        <td><div id="rly7"></div></td>
        <td><div id="rly8"></div></td>
      </tr>
    </table>
    <hr>
    <footer>&copy; 2025 YS Lab Version 0.40</footer>
    <script>
      function updateProgress() {
	  if (progress <= 100) {
              // サーバへリクエストを送信
              const xhr = new XMLHttpRequest();
              xhr.open("GET", "progress-ajax-server.php", true); // サーバのPHPスクリプトにアクセス
              xhr.onreadystatechange = function () {
		  if (xhr.readyState === 4 && xhr.status === 200) {
                      // レスポンスを受け取って進捗を更新
                      const response = JSON.parse(xhr.responseText);
                      progress = response.progress; // サーバからの進捗情報を更新

                      // プログレスバーを更新
                      document.getElementById('progress').style.width = progress + '%';
                      document.getElementById('progress').textContent = progress + '%';
                      if (progress >= 100) {
			  console.log('Progress completed!');
                      } else {
			  setTimeout(updateProgress, 1000); // 1秒後に再度進捗を更新
                      }
		  }
              };
              xhr.send(); // リクエストを送信
	  }
      }
	  
      function updateRUNCND() {
	  const xhr = new XMLHttpRequest();
	  xhr.open("GET","ajax-runCND-server.php?M={$data_a.MACADDR}",true);
	  xhr.onreadystatechange = function () {
              if (xhr.readyState === 4 && xhr.status === 200) {
		  // レスポンスを受け取って進捗を更新
		  const response = JSON.parse(xhr.responseText);
		  let cnd = BigInt(response.cnd); // サーバからの進捗情報を更新
		  const rlydata = Number(cnd & 0xffn);
		  const maskedcnd = cnd & 0xfffff000n;
		  console.log("maskedcnd:",maskedcnd);
		  runcnd = document.getElementById('RUNCND');
		  let rlysts = [];
		  {for $i=1 to 8}
		  rlysts[{$i}] = document.getElementById('rly{$i}');
		  {/for}
<!--// -->
		   switch(maskedcnd) {
		   case 2147483648n:
                       runcnd.textContent = "未受信";
                       break;
		   case 0n:
                       runcnd.textContent = "正常動作中 ";
                       break;
		   case 131072n:
		       switch(cnd) {
		       case 131073n:
			   runcnd.textContent = "データ変更中にバッファオーバーフロー";
			   break;
		       case 131074n:
			   runcnd.textContent = "データ変更中";
			   break;
		       case 131075n:
			   runcnd.textContent = "データ読出中にバッファオーバーフロー";
			   break;
		       case 131076n:
			   runcnd.textContent = "データ読出中";
			   break;
		       case 135167n:
			   runcnd.textContent = "RESET中";
			   break;
		       case 132865n:
			   runcnd.textContent = "時計校正中";
			   break;
		       case 132944n:
			   runcnd.textContent = "データ変更値にて初期動作";
			   break;
		       }
		       break;
		   default:
                       runcnd.textContent = cnd ;
		   }
		   setTimeout(updateRUNCND,5000);
		   for (let i=0;i<8;i++) {
		       let p = Math.pow(2,i);
		       if ((rlydata&p)==p) {
			   rlysts[i+1].innerHTML = "<img src=images/lamp-red.gif>";
		       } else {
			   rlysts[i+1].innerHTML = "<img src=images/lamp-no.gif>";
		       }
		   }
		  }
	      };
	      xhr.send(); // リクエストを送信
	  }
	  updateRUNCND();
          </script>
  </body>
</html>
