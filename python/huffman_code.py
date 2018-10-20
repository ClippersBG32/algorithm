#coding=utf8
import struct
import Queue
import pickle

#compressed file
#file header
#     node number: 4 bytes
#     root  index: 4 bytes
#     node  infos: 8bytes * node_info
#        left_child:  4 bytes
#       right_child:  4 bytes,  char is here if left_child is -1
#file content
#     data_length: 8 bytes
#            data: 1 bytes * data_length

class Node(object):
    def __init__(self, index, str, frequency, left = -1, right = -1):
        self.left = left
        self.right = right
        self.index = index
        self.frequency = frequency
        self.str = str


class Huffman(object):
    def __init__(self):
        self.frequency_map = {}
        self.data = None
        self.str_2_bits = {}
        self.tree = None
        self.root = None

    def construct_tree(self):

        self.tree = []

        pq = Queue.PriorityQueue()

        for i, (k, v) in enumerate(self.frequency_map.items()):
            pq.put((v, k, i))
            self.tree.append(Node(i, k, v))

        leaf_num = pq.qsize()
        node_index = leaf_num

        while pq.qsize() > 1:
            cnt1, str1, idx1 = pq.get()
            cnt2, str2, idx2 = pq.get()
            self.tree.append(Node(node_index, 'inner', cnt1 + cnt2, left = idx1, right = idx2))
            pq.put((cnt1 + cnt2, 'i', node_index))
            node_index += 1

        self.root = node_index - 1
        self.get_str_bitcode(node_index - 1, '')


    def get_str_bitcode(self, node_index, str):
        node = self.tree[node_index]

        if node.left < 0:
           self.str_2_bits[node.str] = str
           return

        self.get_str_bitcode(node.left, str + '1')
        self.get_str_bitcode(node.right, str + '0')

    def count_frequency(self):
        self.frequency_map = {}
        for line in self.data:
            char_set = list(set(line))
            char_count = {k:line.count(k) for k in char_set}
            for k, v in char_count.items():
                if self.frequency_map.get(k):
                   self.frequency_map[k] += v
                else:
                   self.frequency_map[k] = v

    def compress_data(self):
        self.count_frequency()
        self.construct_tree()
        compressed_data = self.compress()

        return compressed_data

    def compress(self):
        compressed_data = ''
        for line in self.data:
            for s in line:
                compressed_data += self.str_2_bits[s]
        return compressed_data


    def decompress(self):
        decompressed_data = ''
        node_index = self.root
        node = self.tree[node_index]
        for s in self.data:
            if s == '1':
               node_index = node.left
            elif s == '0':
               node_index = node.right

            node = self.tree[node_index]
            if node.left < 0:
               decompressed_data += node.str
               node = self.tree[self.root]

        return decompressed_data

    def compress_file(self, filename):
        file = open(filename, 'r')

        self.data = file.readlines()
        file.close()

        compressed_data = self.compress_data()
        self.output_as_binary_file(compressed_data, filename)

    def decompress_file(self, filename):
        file = open(filename, 'rb')

        self.get_bitfile_header(file)
        self.data = self.get_data_from_bytes(file)

        file.close()

        print 'Decompressing'
        decompress_data = self.decompress()
        self.output_as_text_file(decompress_data, filename)

    def get_bitfile_header(self, f):
        node_num = struct.unpack('I', f.read(4))[0]
        self.root = struct.unpack('i', f.read(4))[0]
        self.tree = []

        for i in range(node_num):
            left =  int(struct.unpack('i', f.read(4))[0])
            right = int(struct.unpack('i', f.read(4))[0])
            if left < 0:
               str = chr(right)
               right = -1
            else:
               str = 'i'

            self.tree.append(Node(i, str, 0, left, right))

    def set_bitfile_header(self):
        #write the number of nodes
        head_info = ''
        head_info += struct.pack('I', len(self.tree))
        head_info += struct.pack('i', self.root)
        #write node info
        for node in self.tree:
            left, right, str= node.left, node.right, node.str
            head_info += struct.pack('i', left)

            #node is a leaf, encode character into right
            if left < 0:
              head_info += struct.pack('i', ord(str))
            else:
              head_info += struct.pack('i', right)

        return head_info

    def pack_data_to_bytes(self, data):
        data_len = len(data)
        # pack 1/0 string of which length is 8 into a byte, padding zeros if data length can not be divided by 8
        if data_len % 8 != 0:
           data += '0' * (8 - (data_len % 8))

        bytes_data = ''

        for i in range(0, len(data), 8):
            bytes_data += chr(int(data[i: i + 8], 2))

        return struct.pack('I', data_len), struct.pack('%ds'%len(bytes_data), bytes_data)

    def get_data_from_bytes(self, f):

        data_length = struct.unpack('I', f.read(4))[0]

        bytes_data = f.read()
        data = struct.unpack('%ds' %(len(bytes_data)), bytes_data)[0]

        raw_data = ''
        for d in data:
            raw_data += bin(ord(d))[2:].zfill(8)
        return raw_data[:data_length]

    def output_as_binary_file(self, data, filename):
        output_filename = filename + '.bit'

        with open(output_filename, 'wb') as f:
           f.write(self.set_bitfile_header())
           pack_data_len, pack_data = self.pack_data_to_bytes(data)
           f.write(pack_data_len)
           f.write(pack_data)
           f.close()

    def output_as_text_file(self, data, filename):
        output_filename = filename + '.dec'
        f = open(output_filename, 'w')
        f.write(data)
        f.close()
