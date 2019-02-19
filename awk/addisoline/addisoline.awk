BEGIN{
    no = 0 
}
{
    str_buff = $1
    if ( (no == 0) && (str_buff != old_str_buff) ) {
        old_str_buff = str_buff
        no = 1
    }
    else if ( (no != 0) && (str_buff != old_str_buff) ) {
        old_str_buff = str_buff
        print ""
    }
    print $0
}
END{
}
