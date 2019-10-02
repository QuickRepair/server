import json
import urllib.parse
import urllib.request

from TestCases.pytest.Connect.Host import Host


class Connection(object):
    @staticmethod
    def merchant_get_verification(merchant_account):
        url = Host().api_url() + "verification/merchant/" + merchant_account
        return urllib.request.urlopen(urllib.request.Request(url=url))

    @staticmethod
    def merchant_get_token(merchant_account, password):
        url = Host().api_url() + "token/merchant/" + merchant_account
        header = {'password': password}
        return urllib.request.urlopen(urllib.request.Request(url=url, headers=header))

    @staticmethod
    def merchant_get_service(merchant_id):
        url = Host().api_url() + "supported_service/" + merchant_id
        return urllib.request.urlopen(urllib.request.Request(url=url))

    @staticmethod
    def merchant_update_service(merchant_id, token, distance, appliances):
        url = Host().api_url() + "supported_service/" + merchant_id
        header = {'token': token}
        body = urllib.parse.urlencode({json.dumps({
            'max_distance': distance,
            'support_appliance': appliances
        })})
        req = urllib.request.Request(url=url, headers=header, data=body, method='DELETE')
        return urllib.request.urlopen(req)

    @staticmethod
    def merchant_get_order_list(merchant_id, token):
        url = Host().api_url() + "order_list/merchant/" + merchant_id
        header = {'token': token}
        return urllib.request.urlopen(urllib.request.Request(url=url, headers=header))

    @staticmethod
    def merchant_get_order_detail(order_id, token):
        url = Host().api_url() + "order/" + order_id
        header = {'token': token}
        return urllib.request.urlopen(urllib.request.Request(url=url, headers=header))

    @staticmethod
    def merchant_publish_order(token, receiver_id, committer_id, appliance, description):
        url = Host().api_url() + "order"
        header = {'token': token}
        body = urllib.parse.urlencode(json.dumps({
            'receiver': receiver_id,
            'committer': committer_id,
            'appliance': appliance,
            'detail': description
        }))
        return urllib.request.urlopen(urllib.request.Request(url=url, headers=header, data=body))

    @staticmethod
    def merchant_receive_order(order_id, token, merchant_id):
        url = Host().api_url() + "order/" + order_id
        header = {'token': token, 'account': merchant_id}
        body = urllib.parse.urlencode(json.dumps({'state': 'receive'}))
        return urllib.request.urlopen(urllib.request.Request(url=url, headers=header, data=body, method='PUT'))

    @staticmethod
    def merchant_start_repair_order(order_id, token, merchant_id):
        url = Host().api_url() + "order/" + order_id
        header = {'token': token, 'account': merchant_id}
        body = urllib.parse.urlencode(json.dumps({'state': 'start_repair'}))
        return urllib.request.urlopen(urllib.request.Request(url=url, headers=header, data=body, method='PUT'))

    @staticmethod
    def merchant_end_repair_order(order_id, token, merchant_id):
        url = Host().api_url() + "order/" + order_id
        header = {'token': token, 'account': merchant_id}
        body = urllib.parse.urlencode(json.dumps({'state': 'end_repair'}))
        return urllib.request.urlopen(urllib.request.Request(url=url, headers=header, data=body, method='PUT'))

    @staticmethod
    def merchant_pay_order(order_id, token, merchant_id):
        url = Host().api_url() + "order/" + order_id
        header = {'token': token, 'account': merchant_id}
        body = urllib.parse.urlencode(json.dumps({'state': 'pay'}))
        return urllib.request.urlopen(urllib.request.Request(url=url, headers=header, data=body, method='PUT'))

    @staticmethod
    def merchant_reject_order(order_id, token, merchant_id):
        url = Host().api_url() + "order/" + order_id
        header = {'token': token, 'account': merchant_id}
        body = urllib.parse.urlencode(json.dumps({'state': 'reject'}))
        return urllib.request.urlopen(urllib.request.Request(url=url, headers=header, data=body, method='PUT'))

    @staticmethod
    def public_get_merchant_list(longitude, latitude):
        url = Host().api_url() + 'merchant_list/' + longitude + '/' + latitude
        return urllib.request.urlopen(urllib.request.Request(url=url))

    @staticmethod
    def public_get_merchant_service(merchant_id):
        url = Host().api_url() + 'supported_service/' + merchant_id
        return urllib.request.urlopen(urllib.request.Request(url=url))

    @staticmethod
    def public_get_allowed_services():
        url = Host().api_url() + 'allowed_service/'
        return urllib.request.urlopen(urllib.request.Request(url=url))

    @staticmethod
    def customer_get_verification(customer_account):
        url = Host().api_url() + "verification/customer/" + customer_account
        return urllib.request.urlopen(urllib.request.Request(url=url))

    @staticmethod
    def customer_get_token(customer_account, password):
        url = Host().api_url() + "token/customer/" + customer_account
        header = {'password': password}
        return urllib.request.urlopen(urllib.request.Request(url=url, headers=header))

    @staticmethod
    def customer_publish_order(token, receiver_id, committer_id, appliance, description):
        url = Host().api_url() + "order"
        header = {'token': token}
        body = urllib.parse.urlencode(json.dumps({
            'receiver': receiver_id,
            'committer': committer_id,
            'appliance': appliance,
            'detail': description
        }))
        return urllib.request.urlopen(urllib.request.Request(url=url, headers=header, data=body))

    @staticmethod
    def customer_get_order_list(customer_id, token):
        url = Host().api_url() + "order_list/customer/" + customer_id
        header = {'token': token}
        return urllib.request.urlopen(urllib.request.Request(url=url, headers=header))

    @staticmethod
    def customer_get_order_detail(order_id, token):
        url = Host().api_url() + "order/" + order_id
        header = {'token': token}
        return urllib.request.urlopen(urllib.request.Request(url=url, headers=header))

    @staticmethod
    def customer_pay_order(order_id, token, customer_id):
        url = Host().api_url() + "order/" + order_id
        header = {'token': token, 'account': customer_id}
        body = urllib.parse.urlencode(json.dumps({'state': 'pay'}))
        return urllib.request.urlopen(urllib.request.Request(url=url, headers=header, data=body, method='PUT'))
