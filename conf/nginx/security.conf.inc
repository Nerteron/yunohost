ssl_session_timeout 1d;
ssl_session_cache shared:SSL:50m;  # about 200000 sessions
ssl_session_tickets off;

{% if compatibility == "modern" %}
# generated 2026-06-20, TLSRef Guideline v6.0, nginx 1.26.3, OpenSSL 3.5.6, modern config, gitrev=9c09b2d
# https://configurator.tlsref.org/#server=nginx&version=1.26.3&config=modern&openssl=3.5.6&guideline=6.0
ssl_protocols TLSv1.3;
{% else %}
# Ciphers with intermediate compatibility
# generated 2026-06-20, TLSRef Guideline v6.0, nginx 1.26.3, OpenSSL 3.5.6, intermediate config, HSTS, gitrev=9c09b2d
# https://configurator.tlsref.org/#server=nginx&version=1.26.3&config=intermediate&openssl=3.5.6&hsts&guideline=6.0
ssl_protocols TLSv1.2 TLSv1.3;
ssl_ciphers ECDHE-ECDSA-AES128-GCM-SHA256:ECDHE-RSA-AES128-GCM-SHA256:ECDHE-ECDSA-AES256-GCM-SHA384:ECDHE-RSA-AES256-GCM-SHA384:ECDHE-ECDSA-CHACHA20-POLY1305:ECDHE-RSA-CHACHA20-POLY1305;
# Pre-defined FFDHE group (RFC 7919)
# From https://ssl-config.mozilla.org/ffdhe2048.txt
# https://security.stackexchange.com/a/149818
ssl_dhparam /usr/share/yunohost/ffdhe2048.pem;
{% endif %}

ssl_ecdh_curve X25519MLKEM768:X25519:prime256v1:secp384r1;
ssl_prefer_server_ciphers off;


# Follows the Web Security Directives from the Mozilla Dev Lab and the Mozilla Obervatory + Partners
# https://wiki.mozilla.org/Security/Guidelines/Web_Security
# https://observatory.mozilla.org/
{% if experimental == "True" %}
more_set_headers "Content-Security-Policy : upgrade-insecure-requests; default-src https: data: blob: ; object-src https: data: 'unsafe-inline'; style-src https: data: 'unsafe-inline' ; script-src https: data: 'unsafe-inline' 'unsafe-eval'; worker-src 'self' blob:;";
{% else %}
more_set_headers "Content-Security-Policy : upgrade-insecure-requests";
{% endif %}
more_set_headers "X-Content-Type-Options : nosniff";
more_set_headers "X-XSS-Protection : 1; mode=block";
more_set_headers "X-Download-Options : noopen";
more_set_headers "X-Permitted-Cross-Domain-Policies : none";
more_set_headers "X-Frame-Options : SAMEORIGIN";

# Disable the disaster privacy thing that is FLoC
{% if experimental == "True" %}
more_set_headers "Permissions-Policy : fullscreen=(), geolocation=(), payment=(), accelerometer=(), battery=(), magnetometer=(), usb=(), interest-cohort=()";
# Force HTTPOnly and Secure for all cookies
# Disabled because incompatible with the new cookie management system
# proxy_cookie_path ~$ "; HTTPOnly; Secure;";
{% else %}
more_set_headers "Permissions-Policy : interest-cohort=()";
{% endif %}

# Disable gzip to protect against BREACH
# Read https://trac.nginx.org/nginx/ticket/1720 (text/html cannot be disabled!)
gzip off;
