<?php

function http_get($url, $header, $refer,$proxy=null,$cookie=null)
{
      $ch = curl_init();
      curl_setopt($ch, CURLOPT_URL, $url);
      if ($header) curl_setopt($ch, CURLOPT_HTTPHEADER, $header);
      if ($refer) curl_setopt($ch, CURLOPT_REFERER, $refer);
      if ($proxy) curl_setopt($ch,CURLOPT_PROXY,$proxy);
      if ($cookie) curl_setopt($ch,CURLOPT_COOKIE,$cookie);

      curl_setopt($ch, CURLOPT_RETURNTRANSFER,1); // return into a variable
      curl_setopt($ch, CURLOPT_USERAGENT, 'Internet Explorer 9.0 Mozilla/5.0 (compatible; MSIE 9.0; Windows NT 6.1; Trident/5.0)');
      curl_setopt($ch, CURLOPT_ENCODING, 'gzip,deflate');
      curl_setopt($ch, CURLOPT_FOLLOWLOCATION, 1);// allow redirects

      $res = curl_exec($ch);

	  $errno = curl_errno($ch);

      $info = curl_getinfo($ch);

      curl_close($ch);

      if($info['http_code'] != 200)
             return false;

	  if($errno!=0)
		  return false;

      return $res;
}

function http_post($url,$data,$header)
{
    $ch=curl_init();
    if($header)
        curl_setopt($ch,CURLOPT_HTTPHEADER,$header);

    curl_setopt($ch,CURLOPT_RETURNTRANSFER,1);
    curl_setopt($ch,CURLOPT_URL,$url);
    curl_setopt($ch,CURLOPT_POST,1);
    curl_setopt($ch,CURLOPT_POSTFIELDS,$data);

    curl_setopt($ch, CURLOPT_USERAGENT, 'Internet Explorer 9.0 Mozilla/5.0 (compatible; MSIE 9.0; Windows NT 6.1; Trident/5.0)');
    curl_setopt($ch, CURLOPT_ENCODING, 'gzip,deflate');
    curl_setopt($ch, CURLOPT_FOLLOWLOCATION, 1);// allow redirects

    $res=curl_exec($ch);

	$errno = curl_errno($ch);

    $info=curl_getinfo($ch);

    curl_close($ch);

    if($info['http_code']!=200)
            return false;

	if($errno!=0)
		return false;

    return $res;
}

function get_cur_sec()
{
    $time=gettimeofday();
    return $time['sec']*1000+$time['usec']/1000;
}

?>
