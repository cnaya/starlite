$(document).ready(function () {
    $('#login-submit').click(function (e) {
        e.preventDefault();
        var email = $('#inputEmail').val(), password = $('#inputPassword').val();
        if (email.length == 0 || password.length == 0) {
            layer.alert('表单未填写完整。');
            return;
        }
        var dialog = layer.load();
        $.post("/blog/login", {'inputEmail': email, 'inputPassword': password}, function (result) {
            layer.close(dialog)
            layer.alert(result.message);
            if (result.ok) {
                setTimeout(function () {
                    if (document.referrer != null) {
                        var regex = new RegExp('/blog/logout')
                        if (regex.test(document.referrer)) {
                            location.href = '/blog'
                        } else {
                            location.href = document.referrer;
                        }
                    } else {
                        location.href = '/blog/edit';
                    }
                }, 2000);
            }
        });
    });
    $('#search').click(function (e) {
        e.preventDefault();
        layer.prompt({title: "搜索"}, function (text, index) {
            layer.close(index);
            location.href = '/blog/search?key=' + text + "&page=1";
        });
    });

    $(window).scroll(function () {
        if ($(this).scrollTop() > 50) {
            $('#back-to-top').fadeIn();
        } else {
            $('#back-to-top').fadeOut();
        }
    });
    // scroll body to 0px on click
    $('#back-to-top').click(function () {
        $('#back-to-top').tooltip('hide');
        $('body,html').animate({
            scrollTop: 0
        }, 800);
        return false;
    });

    $('#back-to-top').tooltip('show');
    $.post('/blog/login', function (result) {
        if (typeof (result.login) != 'undefined') {
            $('.navbar-right li a:first').text('退出').attr('href', '/blog/logout');
        }
    });

});

