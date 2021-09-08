
<?php

$indicesServer = array('GATEWAY_INTERFACE','SERVER_NAME','SERVER_SOFTWARE','SERVER_PROTOCOL',
'REQUEST_METHOD','REQUEST_TIME','REQUEST_TIME_FLOAT','QUERY_STRING','DOCUMENT_ROOT','HTTP_ACCEPT',
'HTTP_ACCEPT_ENCODING','HTTP_ACCEPT_LANGUAGE','HTTP_CONNECTION','HTTP_HOST','SCRIPT_FILENAME','REQUEST_URI') ;

echo '<html lang="en"><head><meta charset="utf-8"><title>Webserv - CGI params</title>' ;
echo '<style>body{margin: 2vh 8vw;}section{margin: 10vh 4vw;}' ;
echo '.text-center{text-align: center;}.text-justify{text-align: justify;}' ;
echo '.text-emphase{font-style: oblique;}.hr-invisible{visibility: hidden; height: 20px;}' ;
echo 'table{border: solid 1px;}td{border: solid 1px; padding: 5px;}.tdl{width: 20%;}\r\n' ;
echo '.tdr{width: 80%;}.box{padding: 20px; margin: 20px; text-align: center; font-style: oblique;}' ;
echo '.box-advice{background-color:rgb(255, 240, 153);}.box-warning{background-color:rgb(255, 146, 134);}' ;
echo '.box-info{background-color:rgb(144, 212, 254);}</style></head>' ;
echo '<body><h1 class="text-center">Webserv - CGI params</h1><div><table>' ;
foreach ($indicesServer as $arg) {
    if (isset($_SERVER[$arg])) {
        echo '<tr><td>'.$arg.'</td><td>' . $_SERVER[$arg] . '</td></tr>' ;
    }
    else {
        echo '<tr><td>'.$arg.'</td><td>-</td></tr>' ;
    }
}
echo '</table></div></body></html>' ;

?>